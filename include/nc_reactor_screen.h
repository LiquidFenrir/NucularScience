/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_REACTOR_SCREEN_H
#define NC_REACTOR_SCREEN_H

#include "nc_screen_base.h"
#include "nc_status.h"

#include "bn_sprite_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_array.h"
#include "bn_vector.h"
#include "bn_optional.h"

namespace nc
{

class reactor_screen : public screen
{

public:
    reactor_screen(nc::status& status, bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    nc::status& _status;
    bn::regular_bg_ptr _reactor_background;

    bn::sprite_ptr _cursor_hover_sprite, _cursor_lock_sprite;
    bn::sprite_ptr _shop_meta_sprite;
    bn::vector<bn::sprite_ptr, SHOP_HEIGHT * SHOP_WIDTH> _shop_sprites;
    bn::vector<bn::sprite_ptr, REACTOR_HEIGHT * REACTOR_WIDTH> _reactor_sprites;

    bn::sprite_move_by_action _cursor_move_left_reactor_action;
    bn::sprite_move_by_action _cursor_move_right_reactor_action;
    bn::sprite_move_by_action _cursor_move_up_reactor_action;
    bn::sprite_move_by_action _cursor_move_down_reactor_action;

    bn::sprite_move_by_action _cursor_move_left_shop_action;
    bn::sprite_move_by_action _cursor_move_right_shop_action;
    bn::sprite_move_by_action _cursor_move_up_shop_action;
    bn::sprite_move_by_action _cursor_move_down_shop_action;
    bn::sprite_move_by_action _cursor_move_up_section_shop_action;
    bn::sprite_move_by_action _cursor_move_down_section_shop_action;

    bn::sprite_animate_action<2> _cursor_hover_loop_action;
    bn::sprite_animate_action<2> _cursor_lock_loop_action;

    bool _holding_a{false};
    index_t _started_hold_at{0};

    void _reset_cursor_positions();
};
}

#endif
