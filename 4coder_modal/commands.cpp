void set_keymap(Application_Links *app, int map)
{
     View_Summary view = get_active_view(app, AccessAll);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, AccessAll);

    if (buffer.exists)
        buffer_set_setting(app, &buffer, BufferSetting_MapID, map);
}
 
CUSTOM_COMMAND_SIG(modal_enter_global)
{
    set_keymap(app, mapid_global);
    
    Theme_Color colors[] = {
        { Stag_Cursor, 0xffff5533 },
        { Stag_At_Cursor, 0xff00aacc },
        { Stag_Margin_Active, 0xffff5533 },
    };
    
    set_theme_colors(app, colors, ArrayCount(colors));
}

CUSTOM_COMMAND_SIG(modal_enter_insert)
{
    set_keymap(app, mapid_insert);
    
    Theme_Color colors[] = {
        { Stag_Cursor, 0xff80ff80 },
        { Stag_At_Cursor, 0xff293134 },
        { Stag_Margin_Active, 0xff80ff80 },
    };
    
    set_theme_colors(app, colors, ArrayCount(colors));
}

CUSTOM_COMMAND_SIG(modal_enter_replace)
{
    set_keymap(app, mapid_replace);
    
    Theme_Color colors[] = {
        { Stag_Cursor, 0xff005ffd },
        { Stag_At_Cursor, 0xff0f8dfc },
        { Stag_Margin_Active, 0xff005ffd },
        };
    
    set_theme_colors(app, colors, ArrayCount(colors));
    }

CUSTOM_COMMAND_SIG(modal_live_replace)
{
    exec_command(app, delete_char);
    exec_command(app, write_character);
}

CUSTOM_COMMAND_SIG(modal_write_and_enter_insert)
{
    exec_command(app, write_and_auto_tab);
    exec_command(app, modal_enter_insert);
}

CUSTOM_COMMAND_SIG(modal_write_and_insert_complete)
{
    exec_command(app, open_long_braces);
    exec_command(app, modal_enter_insert);
}
