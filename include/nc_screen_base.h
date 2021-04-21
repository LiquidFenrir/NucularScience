/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_SCREEN_H
#define NC_SCREEN_H

#include "nc_scene.h"
#include "bn_string_view.h"
#include "bn_regular_bg_ptr.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"

namespace bn
{
    class sprite_text_generator;
}

namespace nc
{

enum class scene_type;

class screen : public scene
{

public:
    virtual ~screen() = default;

protected:
    screen(const bn::string_view& title, bn::sprite_text_generator& text_generator);

    void _set_title_text(const bn::string_view& title, bn::sprite_text_generator& text_generator);

    bn::regular_bg_ptr _top_bar_background;
    bn::vector<bn::sprite_ptr, 4> _title_text;
};

}

#endif