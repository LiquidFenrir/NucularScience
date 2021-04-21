/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_research_screen.h"
#include "nc_status.h"

#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_upgrade_back.h"
#include "bn_sprite_items_cursor_upgrades.h"
#include "bn_regular_bg_items_upgrades_screen.h"

namespace nc
{

research_screen::research_screen(nc::status& status, bn::sprite_text_generator& text_generator) :
    screen(UPGRADE_PAGE_NAME[0], text_generator),
    _status(status),
    _text_generator(text_generator),
    _research_bg(bn::regular_bg_items::upgrades_screen.create_bg(0, 0)),
    _current_page(0),
    _cursor_sprite(bn::sprite_items::cursor_upgrades.create_sprite(0, -24)),
    _cursor_loop_action(bn::create_sprite_animate_action_forever(_cursor_sprite, 15, bn::sprite_items::cursor_upgrades.tiles_item(), 0, 1)),
    _cursor_move_left_action(_cursor_sprite, -32, 0),
    _cursor_move_right_action(_cursor_sprite, +32, 0),
    _cursor_move_up_action(_cursor_sprite, 0, -32),
    _cursor_move_down_action(_cursor_sprite, 0, +32)
{
    auto& cursor = _status.get_cursor();
    cursor.column = 0;
    cursor.row = 0;
    cursor.position = Cursor::Position::Upgrades;

    _research_bg.set_priority(3);
    for(int i = 0; i < _back_sprites.max_size(); ++i)
    {
        const int column = i / 2;
        const int row = i % 2;
        const int x = -(UPGRADES_WIDTH * 32 / 2) + (column * 32);
        const int y = -48 + (row * 32);
        _back_sprites.push_back(bn::sprite_items::upgrade_back.create_sprite(x, y));
        _back_sprites.back().set_bg_priority(3);
    }

    _reset_cursor_positions();
    _cursor_sprite.set_bg_priority(0);
}

UpdateResult research_screen::update()
{
    UpdateResult result;

    move_info_t move_info{0};
    _cursor_loop_action.update();

    if(bn::keypad::a_pressed())
    {
        bn::sound_items::buy_upgrade.play(0.5);
    }
    else if(bn::keypad::left_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_left();
    }
    else if(bn::keypad::right_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_right();
    }
    else if(bn::keypad::up_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_up();
    }
    else if(bn::keypad::down_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_down();
    }
    else if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
    {
        if(bn::keypad::l_pressed())
        {
            result = scene_type::REACTOR;
        }
        else
        {
            result = scene_type::INDEX;
        }
        _title_text.clear();
    }

    if(move_info & nc::status::RESET_MOVE)
    {
        _reset_cursor_positions();
    }
    else
    {
        if(move_info & nc::status::CURSOR_LEFT_MOVE)
        {
            _cursor_move_left_action.update();
        }
        else if(move_info & nc::status::CURSOR_RIGHT_MOVE)
        {
            _cursor_move_right_action.update();
        }
        else if(move_info & nc::status::CURSOR_UP_MOVE)
        {
            _cursor_move_up_action.update();
        }
        else if(move_info & nc::status::CURSOR_DOWN_MOVE)
        {
            _cursor_move_down_action.update();
        }
    }

    return result;
}


void research_screen::_reset_cursor_positions()
{
    auto& cursor = _status.get_cursor();
    const int x = -(UPGRADES_WIDTH * 32 / 2) + (cursor.column * 32);
    const int y = -48 + (cursor.row * 32);
    _cursor_sprite.set_position(x, y);
}

}
