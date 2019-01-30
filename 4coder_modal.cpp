#include "4coder_default_include.cpp"
#include "4coder_modal/default_bindings.cpp"

START_HOOK_SIG(modal_start)
{
    named_maps = named_maps_values;
    named_map_count = ArrayCount(named_maps_values);
    
    default_4coder_initialize(app);
    default_4coder_side_by_side_panels(app, files, file_count);
    
    exec_command(app, modal_enter_global);
    exec_command(app, toggle_fullscreen);
    
    if (global_config.automatically_load_project){
        load_project(app);
    }
    
    return 0;
}

OPEN_FILE_HOOK_SIG(modal_file_settings){
    // NOTE(allen|a4.0.8): The get_parameter_buffer was eliminated
    // and instead the buffer is passed as an explicit parameter through
    // the function call.  That is where buffer_id comes from here.
    Buffer_Summary buffer = get_buffer(app, buffer_id, AccessAll);
    Assert(buffer.exists);
    
    bool32 treat_as_code = false;
    bool32 treat_as_todo = false;
    bool32 lex_without_strings = false;
    
    CString_Array extensions = get_code_extensions(&global_config.code_exts);
    
    Parse_Context_ID parse_context_id = 0;
    
    if (buffer.file_name != 0 && buffer.size < (16 << 20)){
        String name = make_string(buffer.file_name, buffer.file_name_len);
        String ext = file_extension(name);
        for (int32_t i = 0; i < extensions.count; ++i){
            if (match(ext, extensions.strings[i])){
                treat_as_code = true;
                
                if (match(ext, "cs")){
                    if (parse_context_language_cs == 0){
                        init_language_cs(app);
                    }
                    parse_context_id = parse_context_language_cs;
                }
                
                if (match(ext, "java")){
                    if (parse_context_language_java == 0){
                        init_language_java(app);
                    }
                    parse_context_id = parse_context_language_java;
                }
                
                if (match(ext, "rs")){
                    if (parse_context_language_rust == 0){
                        init_language_rust(app);
                    }
                    parse_context_id = parse_context_language_rust;
                    lex_without_strings = true;
                }
                
                if (match(ext, "cpp") || match(ext, "h") || match(ext, "c") || match(ext, "hpp") || match(ext, "cc")){
                    if (parse_context_language_cpp == 0){
                        init_language_cpp(app);
                    }
                    parse_context_id = parse_context_language_cpp;
                }
                
                // TODO(NAME): Real GLSL highlighting
                if (match(ext, "glsl")){
                    if (parse_context_language_cpp == 0){
                        init_language_cpp(app);
                    }
                    parse_context_id = parse_context_language_cpp;
                }
                
                // TODO(NAME): Real Objective-C highlighting
                if (match(ext, "m")){
                    if (parse_context_language_cpp == 0){
                        init_language_cpp(app);
                    }
                    parse_context_id = parse_context_language_cpp;
                }
                
                break;
            }
        }
        
        if (!treat_as_code){
            treat_as_todo = match_insensitive(front_of_directory(name), "todo.txt");
        }
    }
    
    int32_t map_id = mapid_global;
    int32_t map_id_query = 0;
    
    buffer_set_setting(app, &buffer, BufferSetting_MapID, default_lister_ui_map);
    buffer_get_setting(app, &buffer, BufferSetting_MapID, &map_id_query);
    Assert(map_id_query == default_lister_ui_map);
    
    buffer_set_setting(app, &buffer, BufferSetting_WrapPosition, global_config.default_wrap_width);
    buffer_set_setting(app, &buffer, BufferSetting_MinimumBaseWrapPosition, global_config.default_min_base_width);
    buffer_set_setting(app, &buffer, BufferSetting_MapID, map_id);
    buffer_get_setting(app, &buffer, BufferSetting_MapID, &map_id_query);
    Assert(map_id_query == map_id);
    buffer_set_setting(app, &buffer, BufferSetting_ParserContext, parse_context_id);
    
    // NOTE(allen): Decide buffer settings
    bool32 wrap_lines = true;
    bool32 use_virtual_whitespace = false;
    bool32 use_lexer = false;
    if (treat_as_todo){
        lex_without_strings = true;
        wrap_lines = true;
        use_virtual_whitespace = true;
        use_lexer = true;
    }
    else if (treat_as_code){
        wrap_lines = global_config.enable_code_wrapping;
        use_virtual_whitespace = global_config.enable_virtual_whitespace;
        use_lexer = true;
    }
    if (match(make_string(buffer.buffer_name, buffer.buffer_name_len), "*compilation*")){
        wrap_lines = false;
    }
    //if (buffer.size >= (192 << 10)){
    if (buffer.size >= (128 << 10)){
        wrap_lines = false;
        use_virtual_whitespace = false;
    }
    
    // NOTE(allen|a4.0.12): There is a little bit of grossness going on here.
    // If we set BufferSetting_Lex to true, it will launch a lexing job.
    // If a lexing job is active when we set BufferSetting_VirtualWhitespace, the call can fail.
    // Unfortunantely without tokens virtual whitespace doesn't really make sense.
    // So for now I have it automatically turning on lexing when virtual whitespace is turned on.
    // Cleaning some of that up is a goal for future versions.
    buffer_set_setting(app, &buffer, BufferSetting_LexWithoutStrings, lex_without_strings);
    buffer_set_setting(app, &buffer, BufferSetting_WrapLine, wrap_lines);
    buffer_set_setting(app, &buffer, BufferSetting_VirtualWhitespace, use_virtual_whitespace);
    buffer_set_setting(app, &buffer, BufferSetting_Lex, use_lexer);
    // no meaning for return
    return(0);
}

inline void
setup_hooks(Bind_Helper *context)
{
    set_hook(context, hook_exit, default_exit);
    set_hook(context, hook_view_size_change, default_view_adjust);
    
    set_start_hook(context, modal_start);
	set_open_file_hook(context, modal_file_settings);
    set_end_file_hook(context, end_file_close_jump_list);
    
    set_command_caller(context, default_command_caller);
    set_render_caller(context, default_render_caller);
    set_input_filter(context, default_suppress_mouse_filter);
    set_scroll_rule(context, smooth_scroll_rule);
    set_buffer_name_resolver(context, default_buffer_name_resolution);
}

extern "C" int32_t
get_bindings(void *data, int32_t size)
{
    Bind_Helper context_ = begin_bind_helper(data, size);
    Bind_Helper *context = &context_;
    
    setup_hooks(context);
    
    bind_shared_mode(context);
    bind_global_mode(context);
    bind_insert_mode(context);
    
    int32_t result = end_bind_helper(context);
    return result;
}
