/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_TITLE_H
#define NC_TITLE_H

#include "bn_optional.h"
#include "bn_vector.h"
#include "bn_array.h"
#include "bn_display.h"
#include "bn_sprite_ptr.h"
#include "bn_blending_actions.h"
#include "bn_sprite_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"

#include "nc_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace nc
{

class status;

class title : public scene
{

public:
    title(nc::status& status, bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    enum class state
    {
        START,
        MENU,
        HIDE_CURSOR,
        END
    };

    nc::status& _status;
    bn::sprite_text_generator& _text_generator;
    bn::vector<bn::sprite_ptr, 3> _continue_text_sprites;
    bn::vector<bn::sprite_ptr, 3> _start_text_sprites;
    bn::vector<bn::sprite_ptr, 3> _how_to_play_sprites;
    bn::vector<bn::fixed, 4> _cursor_points_vec;
    bn::optional<bn::sprite_move_to_action> _cursor_move_action;
    bn::optional<bn::sprite_scale_to_action> _cursor_scale_action;
    bn::optional<bn::blending_intensity_alpha_to_action> _blending_intensity_action;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_transparency_action;

    bn::sprite_ptr _cursor_sprite;
    bn::regular_bg_ptr _logo, _facility, _bg;

    bn::fixed _wavy_angle;
    bn::array<bn::fixed, bn::display::height()> _horizontal_deltas;
    bn::optional<bn::regular_bg_position_hbe_ptr> _logo_wavy_animation;
    bn::optional<bn::regular_bg_move_to_action> _logo_move_action;
    bn::optional<bn::regular_bg_move_to_action> _facility_move_action;
    bn::optional<bn::regular_bg_move_to_action> _bg_move_action;

    int _menu_index;
    state _state;

    void _animate_logo();
    [[nodiscard]] UpdateResult menu();
};

}

#endif
