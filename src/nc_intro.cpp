/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_intro.h"

#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_version.h"
#include "bn_optional.h"
#include "bn_sprite_text_generator.h"
#include "bn_window.h"
#include "bn_display.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_butano_big_silhouette.h"
#include "bn_regular_bg_items_butano_background.h"

namespace nc
{

namespace
{
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_action()
    {
        bn::bg_palettes::set_fade(bn::colors::maroon, 1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }
    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bg_fade_out_action()
    {
        bn::bg_palettes::set_fade(bn::color(14, 0, 0), 0);
        return bn::bg_palettes_fade_to_action(30, 1);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprite_fade_action()
    {
        bn::sprite_palettes::set_fade(bn::colors::maroon, 1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }
    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprite_fade_out_action()
    {
        bn::sprite_palettes::set_fade(bn::color(14, 0, 0), 0);
        return bn::sprite_palettes_fade_to_action(60, 1);
    }
    
    constexpr const int show_hide_frames = 10;

    constexpr const bn::array<bn::color, bn::display::height()> palette_hbe_colors = []{
        bn::array<bn::color, bn::display::height()> result;
        bn::color color(14, 0, 0);
        int redInc = 16;
        int greenInc = 16;
        int blueInc = 8;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            result[index] = color;
            --redInc;
            --greenInc;
            --blueInc;

            if(! redInc)
            {
                color.set_red(color.red() + (index <= 80 ? 1 : -1));
                redInc = 16;
            }

            if(! greenInc)
            {
                color.set_green(bn::max(color.green() + (index <= 80 ? 1 : -1), 0));
                greenInc = 16;
            }

            if(! blueInc)
            {
                color.set_blue(bn::max(color.blue() + (index <= 80 ? 1 : -1), 0));
                blueInc = 16;
            }
        }

        return result;
    }();
}

intro::intro(bn::sprite_text_generator& text_generator) :
    _bg_fade_action(_create_bg_fade_action()),
    _sprite_fade_action(_create_sprite_fade_action()),
    _blending_action(60, 0)
{
    bn::string<32> middle_text("BUTANO ");
    middle_text.append(bn::to_string<4>(bn::version::major()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::minor()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::patch()));

    text_generator.set_center_alignment();
    text_generator.generate(0, -16, "Made with", _text_sprites);
    text_generator.generate(0, 0, middle_text, _text_sprites);
    text_generator.generate(0, 16, "github.com/GValiente/butano", _text_sprites);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_blending_enabled(true);
    }

    bn::regular_bg_ptr bg = bn::regular_bg_items::butano_background.create_bg(0, 0);
    _palette_hbe = bn::bg_palette_color_hbe_ptr::create(bg.palette(), 1, palette_hbe_colors);
    bg.set_priority(3);
    _move_action.emplace(bn::move(bg), 1, -1);
}

UpdateResult intro::update()
{
    UpdateResult result;

    if(!_bg_fade_action)
    {
        if(_bg_fade_out_action->done())
        {
            result = scene_type::TRANSITION_TO_SPLASH;
            _palette_hbe.reset();
            _move_action.reset();
            bn::sprite_palettes::set_fade_intensity(0);
            bn::bg_palettes::set_fade_intensity(0);
        }
        else
        {
            _bg_fade_out_action->update();
            _sprites_fade_out_action->update();
        }
    }
    else if(_bg_fade_action->done())
    {
        if(_counter)
        {
            if(bn::keypad::a_pressed())
            {
                _counter = 0;
            }
            else
            {
                --_counter;
            }
        }
        else
        {
            if(_blending_action.done())
            {
                _text_sprites.clear();
                _bg_fade_action.reset();
                bn::blending::set_transparency_alpha(1);

                _bg_fade_out_action = _create_bg_fade_out_action();
                _sprites_fade_out_action = _create_sprite_fade_out_action();
            }
            else
            {
                _blending_action.update();
            }
        }
    }
    else
    {
        _bg_fade_action->update();
        _sprite_fade_action.update();
    }

    if(_move_action)
    {
        _move_action->update();
    }

    return result;
}

}
