/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_preview_screen.h"

#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"

namespace nc
{

preview_screen::preview_screen(nc::status& status, bn::sprite_text_generator& text_generator) :
    screen("DAILY PREVIEW", text_generator),
    _status(status)
{
    
}

UpdateResult preview_screen::update()
{
    UpdateResult result;

    if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
    {
        if(bn::keypad::l_pressed())
        {
            result = scene_type::INDEX;
        }
        else
        {
            result = scene_type::REACTOR;
        }
        _title_text.clear();
    }

    return result;
}

}
