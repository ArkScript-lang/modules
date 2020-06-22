#include <Ark/Module.hpp>
#include <sf_module.hpp>

// module functions mapping
ARK_API_EXPORT Mapping_t getFunctionsMapping()
{
    Mapping_t map;

    map["sf:window:init"] = sf_window_init;
    map["sf:window:open?"] = sf_window_isopen;
    map["sf:window:close"] = sf_window_close;
    map["sf:window:clear"] = sf_window_clear;
    map["sf:window:display"] = sf_window_display;
    map["sf:window:setFPS"] = sf_window_set_fps;

    map["sf:pollEvent"] = sf_poll_event;
    map["sf:draw"] = sf_draw;

    map["sf:load:texture"] = sf_load_texture;
    map["sf:load:sprite"] = sf_load_sprite;
    map["sf:load:font"] = sf_load_font;

    map["sf:make:text"] = sf_make_text;
    map["sf:set:text"] = sf_set_text;
    map["sf:set:pos"] = sf_setpos;

    map["sf:width"] = sf_width;
    map["sf:height"] = sf_height;

    map["sf:event"] = sf_event;

    return map;
}