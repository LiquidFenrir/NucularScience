/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_tutorial.h"
#include "nc_status.h"

#include "bn_sprite_text_generator.h"
#include "bn_display.h"
#include "bn_keypad.h"

namespace nc
{

namespace {
    constexpr TutorialPage _story_page{
        "STORY",
        {
            "You are a nuclear engineer!",
            "Make power in your reactor!",
            "Create the ultimate energy!",
        }
    };
    constexpr TutorialPage _reactor_page{
        "REACTOR",
        {
            "The reactor produces power.",
            "Design it carefully...",
            "Unless you like meltdowns.",
        }
    };
    constexpr TutorialPage _research_page{
        "RESEARCH",
        {
            "Buy upgrades for components.",
            "Unlock the ultimate source!",
            "Will cost a pretty penny.",
        }
    };
    constexpr TutorialPage _preview_page{
        "PREVIEW",
        {
            "Estimate your production!",
            "Heat, power, and costs.",
            "Bigger is better. Mostly.",
        }
    };
    constexpr TutorialPage _index_page{
        "INDEX",
        {
            "Information on components!",
            "Cost, usage, and stats.",
            "Very useful, use it!",
        }
    };

    constexpr const TutorialPage* _pages[] = {
        &_story_page,
        &_reactor_page,
        &_research_page,
        &_preview_page,
        &_index_page,
    };

    template<typename T, int N>
    constexpr int _array_size(const T (&)[N])
    {
        return N;
    }
    constexpr int _max_page = _array_size(_pages);

    void _enable_blending(bn::ivector<bn::sprite_ptr>& sprites)
    {
        for(bn::sprite_ptr& sprite : sprites)
        {
            sprite.set_blending_enabled(true);
        }
    }

    void _set_position(const bn::fixed x_delta, bn::ivector<bn::sprite_ptr>& sprites)
    {
        for(bn::sprite_ptr& sprite : sprites)
        {
            sprite.set_x(sprite.x() + x_delta);
        }
    }
    constexpr int _animation_frames = 40;
    constexpr int _move_per_frame = 2;
    constexpr int _move_per_animation = _animation_frames * _move_per_frame;
}

tutorial::tutorial(nc::status& status, const scene_type after, bn::sprite_text_generator& text_generator) :
    _status(status),
    _after(after),
    _text_generator(text_generator),
    _page_index(-1),
    _going_in(true),
    _going_out(false)
{
    text_generator.set_center_alignment();
    bn::blending::set_transparency_alpha(0);
}

UpdateResult tutorial::update()
{
    UpdateResult result;

    if(_text_blend_action)
    {
        _set_position(_move_per_frame, _title_sprites);
        _set_position(_move_per_frame, _info_sprites);
        _text_blend_action->update();
        if(_text_blend_action->done())
        {
            _text_blend_action.reset();
        }
    }
    else if(_going_in)
    {
        _going_in = false;
        ++_page_index;
        if(_page_index == _max_page)
        {
            _clear_text();
            bn::blending::set_transparency_alpha(1);
            result = _after;
            _status.mark_tutorial_as_complete();
        }
        else
        {
            _set_text(*_pages[_page_index]);
            _text_blend_action.emplace(_animation_frames, 1);
        }
    }
    else if(_going_out)
    {
        _going_out = false;
        _going_in = true;
        _text_blend_action.emplace(_animation_frames, 0);
    }
    else if(bn::keypad::a_pressed())
    {
        _going_out = true;
    }

    return result;
}

void tutorial::_clear_text()
{
    _title_sprites.clear();
    _info_sprites.clear();
}
void tutorial::_set_text(const TutorialPage& page)
{
    _clear_text();
    _text_generator.generate(-_move_per_animation, 12 - (bn::display::height() / 2), page.title, _title_sprites);

    bn::fixed y = -12;
    for(const auto& line : page.lines)
    {
        _text_generator.generate(-_move_per_animation, y, line, _info_sprites);
        y += 16;
    }

    _enable_blending(_title_sprites);
    _enable_blending(_info_sprites);
}
}
