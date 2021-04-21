/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_transition.h"

namespace nc
{

namespace
{
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_action(const bn::color change_to)
    {
        bn::bg_palettes::set_fade(change_to, 0);
        return bn::bg_palettes_fade_to_action(15, 1);
    }
}

transition::transition(const bn::color change_to, const scene_type after) :
    _change_bg_color_action(_create_bg_action(change_to)),
    _final_color(change_to),
    _after(after)
{

}

UpdateResult transition::update()
{
    UpdateResult result;

    _change_bg_color_action.update();
    if(_change_bg_color_action.done())
    {
        bn::bg_palettes::set_transparent_color(_final_color);
        bn::bg_palettes::set_fade_intensity(0);
        result = _after;
    }

    return result;
}

}
