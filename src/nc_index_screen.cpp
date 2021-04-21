/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_index_screen.h"

#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"

namespace nc
{

index_screen::index_screen(nc::status& status, bn::sprite_text_generator& text_generator) :
    screen("INDEX", text_generator),
    _status(status)
{

}

UpdateResult index_screen::update()
{
    UpdateResult result;

    if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
    {
        if(bn::keypad::l_pressed())
        {
            result = scene_type::RESEARCH;
        }
        else
        {
            result = scene_type::PREVIEW;
        }
        _title_text.clear();
    }

    return result;
}

}
