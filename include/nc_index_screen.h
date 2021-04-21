/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_INDEX_SCREEN_H
#define NC_INDEX_SCREEN_H

#include "nc_screen_base.h"
#include "nc_status_fwd.h"

namespace nc
{

class index_screen : public screen
{

public:
    index_screen(nc::status& status, bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    nc::status& _status;
};

}

#endif
