/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_splash.h"

namespace nc
{

namespace
{
    constexpr int _fade_time = 30;
    constexpr int _stay_time = 45;
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_in_action()
    {
        bn::bg_palettes::set_fade(*bn::bg_palettes::transparent_color(), 1);
        return bn::bg_palettes_fade_to_action(_fade_time, 0);
    }
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_out_action()
    {
        return bn::bg_palettes_fade_to_action(_fade_time, 1);
    }
}

splash::splash(const bn::regular_bg_item& splash_image, const bn::color& to_color, const nc::scene_type after) :
    _bg_fade_in_action(_create_bg_fade_in_action()),
    _bg(splash_image.create_bg(0, 0)),
    _to_color(to_color),
    _after(after)
{
}

UpdateResult splash::update()
{
    UpdateResult result;

    if(_bg_fade_in_action)
    {
        _bg_fade_in_action->update();
        if(_bg_fade_in_action->done())
        {
            _bg_fade_in_action.reset();
            bn::bg_palettes::set_fade_color(_to_color);
            _bg_fade_out_action = _create_bg_fade_out_action();
            _counter = _stay_time;
        }
    }
    else if(_counter != 0)
    {
        --_counter;
    }
    else if(_bg_fade_out_action)
    {
        _bg_fade_out_action->update();
        if(_bg_fade_out_action->done())
        {
            bn::bg_palettes::set_transparent_color(_to_color);
            _bg.reset();
            _bg_fade_out_action.reset();
            bn::bg_palettes::set_fade_intensity(0);
            result = _after;
        }
    }

    return result;
}

}
