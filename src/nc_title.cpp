/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_title.h"
#include "nc_status.h"

#include "bn_math.h"
#include "bn_colors.h"
#include "bn_bg_palettes_actions.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_cursor_title.h"
#include "bn_regular_bg_items_logo.h"
#include "bn_regular_bg_items_outside.h"
#include "bn_regular_bg_items_facility.h"

namespace nc
{

namespace
{
    constexpr bn::fixed _logo_go_from_y = -80;
    constexpr bn::fixed _logo_go_to_y = -8;
    constexpr int _logo_take_frames = 120;
    constexpr bn::fixed _text_x = -28;
    constexpr bn::fixed _text_y = 32;
    constexpr bn::fixed _text_y_half_increment = 6;
    constexpr bn::fixed _text_y_increment = _text_y_half_increment + _text_y_half_increment;

    void _add_text(bn::sprite_text_generator& text_generator, bn::fixed& y, const bn::string_view& text, bn::ivector<bn::fixed>& at_y, bn::ivector<bn::sprite_ptr>& into)
    {
        text_generator.generate(_text_x, y, text, into);
        at_y.push_back(y);
        y += _text_y_increment;
    }

    void _hide_text(bn::ivector<bn::sprite_ptr>& text)
    {
        for(bn::sprite_ptr& sprite : text)
        {
            sprite.set_visible(false);
        }
    }
    void _show_text(bn::ivector<bn::sprite_ptr>& text)
    {
        for(bn::sprite_ptr& sprite : text)
        {
            sprite.set_visible(true);
        }
    }
    void _blend_text(bn::ivector<bn::sprite_ptr>& text)
    {
        for(bn::sprite_ptr& sprite : text)
        {
            sprite.set_blending_enabled(true);
        }
    }
}

title::title(nc::status& status, bn::sprite_text_generator& text_generator) :
    _status(status),
    _text_generator(text_generator),
    _cursor_sprite(bn::sprite_items::cursor_title.create_sprite(0, 0)),
    _logo(bn::regular_bg_items::logo.create_bg(0, _logo_go_from_y)),
    _facility(bn::regular_bg_items::facility.create_bg(0, 72)),
    _bg(bn::regular_bg_items::outside.create_bg(0, 48)),
    _logo_wavy_animation(bn::regular_bg_position_hbe_ptr::create_horizontal(_logo, _horizontal_deltas)),
    _logo_move_action(bn::regular_bg_move_to_action(_logo, _logo_take_frames, bn::fixed_point(0, _logo_go_to_y))),
    _facility_move_action(bn::regular_bg_move_to_action(_facility, _logo_take_frames, bn::fixed_point(0, -48))),
    _bg_move_action(bn::regular_bg_move_to_action(_bg, _logo_take_frames, bn::fixed_point(0, -48))),
    _menu_index{0},
    _state{state::START}
{
    text_generator.set_left_alignment();
    bn::fixed text_y = _text_y;

    _cursor_sprite.set_visible(false);
    if(status.have_started_game())
    {
        _add_text(text_generator, text_y, "CONTINUE", _cursor_points_vec, _continue_text_sprites);
    }
    else
    {
        text_y += _text_y_half_increment;
    }

    _add_text(text_generator, text_y, "NEW GAME", _cursor_points_vec, _start_text_sprites);
    _add_text(text_generator, text_y, "HOW TO PLAY", _cursor_points_vec, _how_to_play_sprites);

    _cursor_sprite.set_bg_priority(0);
    _cursor_sprite.set_position(_how_to_play_sprites[0].x() - 28, _cursor_points_vec.front());

    _logo.set_priority(1);
    _bg.set_priority(2);
    _facility.set_priority(3);

    _hide_text(_continue_text_sprites);
    _hide_text(_start_text_sprites);
    _hide_text(_how_to_play_sprites);

    status.start_bgm();
}

UpdateResult title::update()
{
    _animate_logo();
    return menu();
}

void title::_animate_logo()
{
    if(_state != state::START)
    {
        return;
    }

    _wavy_angle += 15;

    if(_wavy_angle >= 360)
    {
        _wavy_angle -= 360;
    }

    bn::fixed degrees_angle = _wavy_angle;

    for(int index = 0, limit = 48; index < limit; ++index)
    {
        degrees_angle += 7.5;

        if(degrees_angle >= 360)
        {
            degrees_angle -= 360;
        }

        bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 6;
        _horizontal_deltas[index] = desp;
    }

    _logo_wavy_animation->reload_deltas_ref();

    _logo_move_action->update();
    _facility_move_action->update();
    _bg_move_action->update();

    if(_logo_move_action->done())
    {
        _facility_move_action.reset();
        _logo_wavy_animation.reset();
        _logo_move_action.reset();
        _bg_move_action.reset();
        _state = state::MENU;
    }
}

[[nodiscard]] UpdateResult title::menu()
{
    const int cursor_scale_frames = 20;
    const int sprites_hide_frames = 60;
    UpdateResult result;

    if(_cursor_move_action)
    {
        _cursor_move_action->update();

        if(_cursor_move_action->done())
        {
            _cursor_move_action.reset();
        }
    }
    else if(_state == state::MENU)
    {
        if(! _cursor_sprite.visible())
        {
            _cursor_sprite.set_visible(true);
            _cursor_sprite.set_scale(0.01);
            _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, 1);

            _show_text(_continue_text_sprites);
            _show_text(_start_text_sprites);
            _show_text(_how_to_play_sprites);
        }

        if(bn::keypad::a_pressed())
        {
            bn::sound_items::click.play(0.75);

            if(bn::blending::intensity_alpha() > 0)
            {
                _blending_intensity_action.emplace(cursor_scale_frames, 0);
            }

            _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, bn::fixed(0.01));
            _state = state::HIDE_CURSOR;
        }
        else if(bn::keypad::up_pressed() || bn::keypad::down_pressed())
        {
            if(bn::keypad::up_pressed())
            {
                if(_menu_index == 0)
                {
                    _menu_index = _cursor_points_vec.size();
                }
                _menu_index -= 1;
            }
            else
            {
                _menu_index += 1;
                if(_menu_index == _cursor_points_vec.size())
                {
                    _menu_index = 0;
                }
            }

            _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _cursor_points_vec[_menu_index]);
            bn::sound_items::move.play(0.5);
        }
    }

    if(_cursor_scale_action)
    {
        _cursor_scale_action->update();

        if(_cursor_scale_action->done())
        {
            _cursor_scale_action.reset();

            if(_state == state::HIDE_CURSOR)
            {
                _blend_text(_continue_text_sprites);
                _blend_text(_start_text_sprites);
                _blend_text(_how_to_play_sprites);

                _cursor_sprite.set_visible(false);
                _blending_transparency_action.emplace(sprites_hide_frames, 0);
                _state = state::END;
            }
        }
    }

    if(_blending_intensity_action)
    {
        _blending_intensity_action->update();

        if(_blending_intensity_action->done())
        {
            _blending_intensity_action.reset();
        }
    }

    if(_blending_transparency_action)
    {
        _blending_transparency_action->update();

        if(_blending_transparency_action->done())
        {
            _blending_transparency_action.reset();
            _continue_text_sprites.clear();
            _start_text_sprites.clear();
            _how_to_play_sprites.clear();
            bn::blending::set_transparency_alpha(1);

            switch(_cursor_points_vec.size() - _menu_index)
            {
                // distance from the bottom of the menu
                // thus in reverse order from their position
                case 1: // how to play
                    result = scene_type::TITLE_TUTORIAL;
                    break;
                case 2: // start new
                    _status.start_game();
                    if(_status.completed_tutorial())
                    {
                        // already saw the tutorial, just play
                        result = scene_type::TRANSITION_TO_GAME;
                    }
                    else
                    {
                        // give instructions before playing
                        result = scene_type::PLAY_TUTORIAL;
                    }
                    break;
                case 3: // continue
                    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));
                    result = scene_type::TRANSITION_TO_GAME;
                    break;
                default:
                    break;
            }
        }
    }

    return result;
}

}
