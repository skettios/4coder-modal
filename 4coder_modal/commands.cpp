void set_keymap(Application_Links *app, int map)
{
    unsigned int access = AccessAll;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);

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
