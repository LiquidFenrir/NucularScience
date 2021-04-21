/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_TRANSITION_H
#define NC_TRANSITION_H

#include "bn_optional.h"
#include "bn_color.h"
#include "bn_bg_palettes_actions.h"

#include "nc_scene.h"

namespace nc
{

class status;

class transition : public scene
{

public:
    transition(const bn::color change_to, const scene_type after);

    [[nodiscard]] UpdateResult update() final;

private:
    bn::bg_palettes_fade_to_action _change_bg_color_action;
    const bn::color _final_color;
    const scene_type _after;
};

}

#endif
