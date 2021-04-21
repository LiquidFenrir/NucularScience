/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_RESEARCH_SCREEN_H
#define NC_RESEARCH_SCREEN_H

#include "nc_screen_base.h"
#include "nc_status_fwd.h"

#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"

namespace nc
{

class research_screen : public screen
{

public:
    research_screen(nc::status& status, bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    nc::status& _status;
    bn::sprite_text_generator& _text_generator;
    bn::regular_bg_ptr _research_bg;
    index_t _current_page;

    bn::sprite_ptr _cursor_sprite;
    bn::sprite_animate_action<2> _cursor_loop_action;

    bn::sprite_move_by_action _cursor_move_left_action;
    bn::sprite_move_by_action _cursor_move_right_action;
    bn::sprite_move_by_action _cursor_move_up_action;
    bn::sprite_move_by_action _cursor_move_down_action;
    bn::vector<bn::sprite_ptr, UPGRADES_WIDTH * UPGRADES_HEIGHT> _back_sprites;
    bn::vector<bn::sprite_ptr, UPGRADES_WIDTH * UPGRADES_HEIGHT> _icon_sprites;
    bn::vector<bn::sprite_ptr, 8> _price_sprites;
    bn::vector<bn::sprite_ptr, 8> _money_sprites;
    bn::vector<bn::sprite_ptr, 32> _description_sprites;

    void _reset_cursor_positions();
};

}

#endif
