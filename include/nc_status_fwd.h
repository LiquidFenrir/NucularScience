/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_STATUS_FWD_H
#define NC_STATUS_FWD_H

#include "bn_utility.h"
#include "bn_string_view.h"
#include "bn_array.h"

namespace nc
{

using index_t = int;

/*
    * bits: xRlrud
    * x - unused
    * R - set position instead of move
    * l - went left
    * r - went right
    * u - went up
    * d - went down
    */
using move_info_t = unsigned char;

static constexpr inline int SHOP_WIDTH = 3;
static constexpr inline int SHOP_HEIGHT = 6;
static constexpr inline int REACTOR_WIDTH = 10;
static constexpr inline int REACTOR_HEIGHT = 8;
static constexpr inline int UPGRADES_PAGES = 2;
static constexpr bn::array<bn::string_view, UPGRADES_PAGES> UPGRADE_PAGE_NAME{
    "POWER UPGRADES",
    "COOLING UPGRADES",
};
static constexpr inline int UPGRADES_WIDTH = 6;
static constexpr inline int UPGRADES_HEIGHT = 2;

class status;

}

#endif
