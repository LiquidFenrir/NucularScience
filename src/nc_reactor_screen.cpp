/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_reactor_screen.h"

#include "bn_optional.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_cursor_hover.h"
#include "bn_sprite_items_cursor_lock.h"
#include "bn_sprite_items_icons.h"
#include "bn_sprite_items_shop_meta.h"
#include "bn_regular_bg_items_reactor.h"

namespace nc
{

reactor_screen::reactor_screen(nc::status& status, bn::sprite_text_generator& text_generator) :
    screen("REACTOR PLAN", text_generator),
    _status(status),
    _reactor_background(bn::regular_bg_items::reactor.create_bg(0, 0)),
    _cursor_hover_sprite(bn::sprite_items::cursor_hover.create_sprite(0, 0)),
    _cursor_lock_sprite(bn::sprite_items::cursor_lock.create_sprite(0, 0)),
    _shop_meta_sprite(bn::sprite_items::shop_meta.create_sprite(0,0)),

    _cursor_move_left_reactor_action(_cursor_hover_sprite, -16, 0),
    _cursor_move_right_reactor_action(_cursor_hover_sprite, +16, 0),
    _cursor_move_up_reactor_action(_cursor_hover_sprite, 0, -16),
    _cursor_move_down_reactor_action(_cursor_hover_sprite, 0, +16),

    _cursor_move_left_shop_action(_cursor_hover_sprite, -20, 0),
    _cursor_move_right_shop_action(_cursor_hover_sprite, +20, 0),
    _cursor_move_up_shop_action(_cursor_hover_sprite, 0, -14),
    _cursor_move_down_shop_action(_cursor_hover_sprite, 0, +14),

    _cursor_move_up_section_shop_action(_cursor_hover_sprite, 0, -18),
    _cursor_move_down_section_shop_action(_cursor_hover_sprite, 0, +18),

    _cursor_hover_loop_action(bn::create_sprite_animate_action_forever(_cursor_hover_sprite, 15, bn::sprite_items::cursor_hover.tiles_item(), 0, 1)),
    _cursor_lock_loop_action(bn::create_sprite_animate_action_forever(_cursor_lock_sprite, 16, bn::sprite_items::cursor_lock.tiles_item(), 0, 1))
{
    auto& cursor = _status.get_cursor();
    cursor.column = 0;
    cursor.row = 0;
    cursor.position = Cursor::Position::Reactor;

    _cursor_lock_sprite.set_visible(false);
    _cursor_lock_sprite.set_bg_priority(2);
    _cursor_hover_sprite.set_bg_priority(1);

    _shop_meta_sprite.set_visible(false);
    _shop_meta_sprite.set_bg_priority(1);
    _shop_meta_sprite.set_position(-88, +62);

    for(int i = 0; i < _shop_sprites.max_size(); ++i)
    {
        const int column = i % 3;
        const int row = i / 3;
        const int x = column * 20 - 108;
        const int y = row * 15 + (4 * (row / 2)) - 46;
        _shop_sprites.push_back(bn::sprite_items::icons.create_sprite(x, y));
        _shop_sprites.back().set_bg_priority(3);

        if(const auto item = _status.specific_item(i); item)
        {
            // ready to buy -> show icon
            _shop_sprites.back().set_tiles(bn::sprite_items::icons.tiles_item().create_tiles(item->sprite_index()));
        }
        else
        {
            // science needed to unlock -> show '?'
            _shop_sprites.back().set_tiles(bn::sprite_items::icons.tiles_item().create_tiles(0));
        }
    }

    for(int i = 0; i < _reactor_sprites.max_size(); ++i)
    {
        const int column = i / 8;
        const int row = i % 8;
        const int x = (column * 16) - (32 + 8);
        const int y = (row * 16) - (48);
        _reactor_sprites.push_back(bn::sprite_items::icons.create_sprite(x, y));

        _reactor_sprites.back().set_bg_priority(3);
        if(const auto tile = _status.specific_tile(i); tile)
        {
            // has item in tile
            _reactor_sprites.back().set_tiles(bn::sprite_items::icons.tiles_item().create_tiles(tile->sprite_index()));
        }
        else
        {
            // empty
            _reactor_sprites.back().set_tiles(bn::sprite_items::icons.tiles_item().create_tiles(0));
            _reactor_sprites.back().set_visible(false);
        }
    }

    _reset_cursor_positions();
}

UpdateResult reactor_screen::update()
{
    UpdateResult result;

    bool meta_changed = false;
    move_info_t move_info{0};

    const auto early_cursor = _status.get_cursor();
    if(bn::keypad::a_pressed())
    {
        bn::sound_items::click.play(0.75);
        if(early_cursor.position == Cursor::Position::Shop)
        {
            _status.select_shop_item();
            meta_changed = true;
        }
        else if(early_cursor.position == Cursor::Position::Reactor)
        {
            if(_status.selected_item())
            {
                const auto tile_item = _status.set_selected_reactor_tile();
                auto& tile = _reactor_sprites[early_cursor.index()];
                tile.set_tiles(bn::sprite_items::icons.tiles_item().create_tiles(tile_item->sprite_index()));
                tile.set_visible(true);
            }
        }
    }
    else if(!_holding_a && bn::keypad::b_pressed())
    {
        bn::sound_items::unclick.play(0.75);
        if(early_cursor.position == Cursor::Position::Shop)
        {
            _status.deselect_shop_item();
            _shop_meta_sprite.set_visible(false);
        }
        else if(early_cursor.position == Cursor::Position::Reactor)
        {
            const auto index = early_cursor.index();
            if(const auto tile_item = _status.specific_tile(index); tile_item)
            {
                _status.clear_selected_reactor_tile();
                _reactor_sprites[index].set_visible(false);
            }
            else
            {
                _status.deselect_shop_item();
                meta_changed = true;
            }
        }
    }
    else if(!_holding_a && bn::keypad::a_held() && early_cursor.position == Cursor::Position::Reactor)
    {
        _holding_a = true;
        _cursor_lock_sprite.set_position(_cursor_hover_sprite.position());
        _cursor_lock_sprite.set_visible(true);
        _started_hold_at = early_cursor.index();
    }
    else if(bn::keypad::left_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_left(_holding_a);
    }
    else if(bn::keypad::right_pressed())
    {
        bn::sound_items::move.play(0.5);
        move_info = _status.move_cursor_right(_holding_a);
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
    else if(!_holding_a && (bn::keypad::l_pressed() || bn::keypad::r_pressed()))
    {
        if(bn::keypad::l_pressed())
        {
            result = scene_type::PREVIEW;
        }
        else
        {
            result = scene_type::RESEARCH;
        }
        _title_text.clear();
    }

    if(_holding_a && bn::keypad::a_released())
    {
        _holding_a = false;
        _status.swap_selected_reactor_tile_with(_started_hold_at);
        const auto currently_at = early_cursor.index();

        // swap the positions, then the displayed thing
        const auto position_a = _reactor_sprites[_started_hold_at].position();
        const auto position_b = _reactor_sprites[currently_at].position();
        _reactor_sprites[_started_hold_at].set_position(position_b);
        _reactor_sprites[currently_at].set_position(position_a);
        bn::swap(_reactor_sprites[_started_hold_at], _reactor_sprites[currently_at]);
        _cursor_lock_sprite.set_visible(false);
    }

    if(move_info & nc::status::RESET_MOVE)
    {
        _reset_cursor_positions();
    }
    else
    {
        const auto new_cursor = _status.get_cursor();
        if(move_info & nc::status::CURSOR_LEFT_MOVE)
        {
            switch(new_cursor.position)
            {
            case Cursor::Position::Shop:
                _cursor_move_left_shop_action.update();
                break;
            case Cursor::Position::Reactor:
                _cursor_move_left_reactor_action.update();
                break;
            }
        }
        else if(move_info & nc::status::CURSOR_RIGHT_MOVE)
        {
            switch(new_cursor.position)
            {
            case Cursor::Position::Shop:
                _cursor_move_right_shop_action.update();
                break;
            case Cursor::Position::Reactor:
                _cursor_move_right_reactor_action.update();
                break;
            }
        }
        else if(move_info & nc::status::CURSOR_UP_MOVE)
        {
            switch(new_cursor.position)
            {
            case Cursor::Position::Shop:
                if((new_cursor.row & 1) == 1)
                {
                    _cursor_move_up_section_shop_action.update();
                }
                else
                {
                    _cursor_move_up_shop_action.update();
                }
                break;
            case Cursor::Position::Reactor:
                _cursor_move_up_reactor_action.update();
                break;
            }
        }
        else if(move_info & nc::status::CURSOR_DOWN_MOVE)
        {
            switch(new_cursor.position)
            {
            case Cursor::Position::Shop:
                if((new_cursor.row & 1) == 1)
                {
                    _cursor_move_down_section_shop_action.update();
                }
                else
                {
                    _cursor_move_down_shop_action.update();
                }
                break;
            case Cursor::Position::Reactor:
                _cursor_move_down_reactor_action.update();
                break;
            }
        }
    }

    if(meta_changed)
    {
        // we were in the reactor and deselected the item
        if(early_cursor.position == Cursor::Position::Reactor)
        {
            _shop_meta_sprite.set_visible(false);
        }
        // we were in the shop and selected a new item that we can buy
        else if(const auto selected_item = _status.selected_item(); selected_item)
        {
            _shop_meta_sprite.set_visible(true);
            _shop_meta_sprite.set_tiles(bn::sprite_items::shop_meta.tiles_item().create_tiles(1)); // buy
        }
        // we were in the shop and selected a new item that is not researched yet
        else
        {
            _shop_meta_sprite.set_visible(true);
            _shop_meta_sprite.set_tiles(bn::sprite_items::shop_meta.tiles_item().create_tiles(0)); // science needed
        }
    }

    _cursor_hover_loop_action.update();
    _cursor_lock_loop_action.update();

    return result;
}

void reactor_screen::_reset_cursor_positions()
{
    const auto cursor = _status.get_cursor();
    switch(cursor.position)
    {
    case Cursor::Position::Shop:
        _cursor_hover_sprite.set_position(cursor.column * 20 - 108, cursor.row * 16 + (2 * (cursor.row / 2)) - 46);
        break;
    case Cursor::Position::Reactor:
        _cursor_hover_sprite.set_position((cursor.column * 16) - (32 + 8), (cursor.row * 16) - (48));
        break;
    }
}

}
