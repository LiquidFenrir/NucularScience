/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_SPLASH_H
#define NC_SPLASH_H

#include "bn_optional.h"
#include "bn_bg_palettes_actions.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_ptr.h"

#include "nc_scene.h"

namespace nc
{

class splash : public scene
{

public:
    splash();

    [[nodiscard]] UpdateResult update() final;

private:
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_in_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_out_action;
    bn::optional<bn::regular_bg_ptr> _bg;
    int _counter;
};

}

#endif
