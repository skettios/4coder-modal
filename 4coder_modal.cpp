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
    
    return 0;
}

inline void
setup_hooks(Bind_Helper *context)
{
    set_hook(context, hook_exit, default_exit);
    set_hook(context, hook_view_size_change, default_view_adjust);
    
    set_start_hook(context, modal_start);
    //set_open_file_hook(context, default_file_settings);
    //set_new_file_hook(context, default_new_file);
    //set_save_file_hook(context, default_file_save);
    
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
