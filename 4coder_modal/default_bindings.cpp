			enum modal_mapid
{
    mapid_shared = default_maps_count,
    mapid_insert,
	mapid_replace,
};

#include "commands.cpp"

static void
bind_shared_mode(Bind_Helper *context)
{
    begin_map(context, mapid_shared);
    {
        inherit_map(context, mapid_nomap);
        
        bind(context, key_left, MDFR_NONE, move_left);
        bind(context, key_right, MDFR_NONE, move_right);
        bind(context, key_up, MDFR_NONE, move_up);
        bind(context, key_down, MDFR_NONE, move_down);
        bind(context, key_end, MDFR_NONE, seek_end_of_line);
        bind(context, key_home, MDFR_NONE, seek_beginning_of_line);
        bind(context, key_page_up, MDFR_NONE, page_up);
        bind(context, key_page_down, MDFR_NONE, page_down);
        bind(context, key_f11, MDFR_SHIFT, toggle_fullscreen);
        bind(context, key_mouse_wheel, MDFR_NONE, mouse_wheel_scroll);
        bind(context, key_mouse_wheel, MDFR_SHIFT, mouse_wheel_change_face_size);
		bind(context, key_left, MDFR_SHIFT, seek_alphanumeric_or_camel_left);
        bind(context, key_right, MDFR_SHIFT, seek_alphanumeric_or_camel_right);
        bind(context, key_up, MDFR_SHIFT, seek_whitespace_up_end_line);
        bind(context, key_down, MDFR_SHIFT, seek_whitespace_down_end_line);
        bind(context, key_del, MDFR_NONE, delete_char);
        bind(context, key_back, MDFR_NONE, backspace_char);
        bind(context, key_del, MDFR_SHIFT, delete_word);
        bind(context, key_back, MDFR_SHIFT, backspace_word);
        bind(context, key_esc, MDFR_NONE, modal_enter_global);
    }
    end_map(context);
}

static void
bind_global_mode(Bind_Helper *context)
{
    begin_map(context, mapid_global);
    {
        inherit_map(context, mapid_shared);
        
        bind(context, '\t', MDFR_NONE, modal_enter_insert);
        bind(context, 'o', MDFR_NONE, interactive_open_or_new);
        bind(context, 'n', MDFR_NONE, interactive_new);
        bind(context, ';', MDFR_NONE, change_active_panel);
        bind(context, 'j', MDFR_NONE, list_all_functions_all_buffers_lister);
        bind(context, ' ', MDFR_NONE, set_mark);
        bind(context, 'b', MDFR_NONE, build_in_build_panel);
        bind(context, 'c', MDFR_NONE, copy);
        bind(context, 'r', MDFR_NONE, modal_enter_replace);
        bind(context, 'd', MDFR_NONE, delete_range);
        bind(context, 'D', MDFR_NONE, delete_line);
        bind(context, 'E', MDFR_NONE, exit_4coder);
        bind(context, 'f', MDFR_NONE, search);
        bind(context, 'F', MDFR_NONE, list_all_locations);
        bind(context, 'g', MDFR_NONE, goto_line);
        bind(context, 'K', MDFR_NONE, kill_buffer);
        bind(context, 'w', MDFR_NONE, save);
        bind(context, 'v', MDFR_NONE, paste_and_indent);
        bind(context, 'V', MDFR_NONE, paste_next_and_indent);
        bind(context, 'x', MDFR_NONE, cut);
        bind(context, 'u', MDFR_NONE, undo);
        bind(context, 'X', MDFR_NONE, command_lister);
        bind(context, 'y', MDFR_NONE, redo);
        bind(context, 'i', MDFR_NONE, interactive_switch_buffer);
        bind(context, 'h', MDFR_NONE, project_go_to_root_directory);
        bind(context, ',', MDFR_NONE, goto_prev_jump_sticky);
        bind(context, '.', MDFR_NONE, goto_next_jump_sticky);
        bind(context, '<', MDFR_NONE, goto_first_jump_sticky);
        bind(context, '?', MDFR_NONE, comment_line_toggle);
        bind(context, '{', MDFR_NONE, modal_write_and_insert_complete);
        }
    end_map(context);
    
    begin_map(context, default_lister_ui_map);
    {
        bind_vanilla_keys(context, lister__write_character);
        bind(context, key_esc, MDFR_NONE, lister__quit);
        bind(context, '\n', MDFR_NONE, lister__activate);
        bind(context, '\t', MDFR_NONE, lister__activate);
        bind(context, key_back, MDFR_NONE, lister__backspace_text_field);
        bind(context, key_up, MDFR_NONE, lister__move_up);
        bind(context, 'k', MDFR_ALT, lister__move_up);
        bind(context, key_page_up, MDFR_NONE, lister__move_up);
        bind(context, key_down, MDFR_NONE, lister__move_down);
        bind(context, 'j', MDFR_ALT, lister__move_down);
        bind(context, key_page_down, MDFR_NONE, lister__move_down);
        bind(context, key_mouse_wheel, MDFR_NONE, lister__wheel_scroll);
        bind(context, key_mouse_left, MDFR_NONE, lister__mouse_press);
        bind(context, key_mouse_left_release, MDFR_NONE, lister__mouse_release);
        bind(context, key_mouse_move, MDFR_NONE, lister__repaint);
        bind(context, key_animate, MDFR_NONE, lister__repaint);
    }
    end_map(context);
}

static void
bind_insert_mode(Bind_Helper *context)
{
    begin_map(context, mapid_insert);
    {
        inherit_map(context, mapid_shared);
        
        bind_vanilla_keys(context, write_character);
		bind(context, '\n', MDFR_NONE, write_and_auto_tab);
        bind(context, '\t', MDFR_NONE, word_complete);
        bind(context, '}', MDFR_NONE, write_and_auto_tab);
        bind(context, ')', MDFR_NONE, write_and_auto_tab);
        bind(context, ']', MDFR_NONE, write_and_auto_tab);
        bind(context, ';', MDFR_NONE, write_and_auto_tab);
        bind(context, '#', MDFR_NONE, write_and_auto_tab);
        }
    end_map(context);
}

static void
bind_replace_mode(Bind_Helper *context)
{
    begin_map(context, mapid_replace);
    {
        inherit_map(context, mapid_shared);
        bind_vanilla_keys(context, modal_live_replace);
        }
    end_map(context);
}
