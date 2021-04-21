/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_splash.h"

#include "bn_colors.h"
#include "bn_regular_bg_items_splash.h"
// #include "bn_music_items.h"

namespace nc
{

namespace
{
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_in_action()
    {
        bn::bg_palettes::set_fade(bn::color(23, 21, 27), 1);
        return bn::bg_palettes_fade_to_action(40, 0);
    }
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_out_action()
    {
        return bn::bg_palettes_fade_to_action(40, 1);
    }
}

splash::splash() :
    _bg_fade_in_action(_create_bg_fade_in_action()),
    _bg(bn::regular_bg_items::splash.create_bg(0, 0))
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
            bn::bg_palettes::set_fade_color(bn::color(0, 18, 31));
            _bg_fade_out_action = _create_bg_fade_out_action();
            _counter = 40;
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
            bn::bg_palettes::set_transparent_color(bn::color(0, 18, 31));
            _bg.reset();
            _bg_fade_out_action.reset();
            bn::bg_palettes::set_fade_intensity(0);
            result = scene_type::TITLE;
            // bn::music_items::bgm.play(0.375, true);
        }
    }

    return result;
}

}
