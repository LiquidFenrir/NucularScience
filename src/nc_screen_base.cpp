/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_screen_base.h"

#include "bn_display.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_items_top_bar.h"
#include "bn_sprite_text_generator.h"

namespace nc
{

screen::screen(const bn::string_view& title, bn::sprite_text_generator& text_generator) :
    _top_bar_background(bn::regular_bg_items::top_bar.create_bg(0, 0))
{
    _top_bar_background.set_priority(3);
    _set_title_text(title, text_generator);
}


void screen::_set_title_text(const bn::string_view& title, bn::sprite_text_generator& text_generator)
{
    _title_text.clear();
    text_generator.set_center_alignment();
    text_generator.generate(0, 8 - (bn::display::height() / 2), title, _title_text);
}

}
