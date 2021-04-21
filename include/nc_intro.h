/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_INTRO_H
#define NC_INTRO_H

#include "bn_optional.h"
#include "bn_sprite_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_bg_palette_color_hbe_ptr.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_blending_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"
#include "nc_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace nc
{

class intro : public scene
{

public:
    intro(bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_action;
    bn::sprite_palettes_fade_to_action _sprite_fade_action;
    bn::blending_transparency_alpha_to_action _blending_action;
    bn::vector<bn::sprite_ptr, 20> _text_sprites;

    bn::optional<bn::regular_bg_move_by_action> _move_action;
    bn::optional<bn::bg_palette_color_hbe_ptr> _palette_hbe;
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;

    int _counter{60 * 4};

    void _set_visible();
};

}

#endif
