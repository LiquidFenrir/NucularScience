/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_STATUS_H
#define NC_STATUS_H

#include "nc_status_fwd.h"
#include "bn_array.h"
#include "bn_optional.h"

namespace nc
{

struct ItemType {
    enum class Type : int {
        // fuel
        UraniumRod,
        PlutoniumRod,
        CaliforniumRod,
        FantasiumRod,
        UnobtainiumRod,
        BlackHoliumRod,

        // heat vents
        BasicVent,
        SuperVent,

        // heat plating
        BasicPlating,
        SuperPlating,

        // coolant cells
        BasicCoolant,
        SuperCoolant,

        // amplifiers
        BasicAmplifier,
        SuperAmplifier,

        // Battery
        BasicBattery,
        SuperBattery,

        // end-game
        BlackHoleStabilizer,
        EnergyExtractor,
    };
    int sprite_index() const;

    static Type from_number(const int value);
    static int to_number(const Type value);

    ItemType(const Type type);
    Type get_type() const;

private:
    Type _type;
};

struct Cursor {
    enum class Position : index_t {
        Shop = 0,
        Reactor = 1,
        Upgrades = 2,
    };
    Position position = Position::Reactor;
    index_t row = 0;
    index_t column = 0;

    index_t index() const;
};

using Price_t = unsigned long long;

class status {

public:
    status();
    void initialize();
    void save(bool game_in_progress);

    void start_bgm();
    void stop_bgm();

    [[nodiscard]] bool have_started_game() const;
    [[nodiscard]] bool completed_tutorial() const;
    void mark_tutorial_as_complete();
    void start_game();
    void complete_game();

    static constexpr move_info_t RESET_MOVE = 0x10;
    static constexpr move_info_t CURSOR_LEFT_MOVE = 0x08;
    static constexpr move_info_t CURSOR_RIGHT_MOVE = 0x04;
    static constexpr move_info_t CURSOR_UP_MOVE = 0x02;
    static constexpr move_info_t CURSOR_DOWN_MOVE = 0x01;

    [[nodiscard]] move_info_t move_cursor_left(const bool keep_in_reactor = false);
    [[nodiscard]] move_info_t move_cursor_right(const bool keep_in_reactor = false);
    [[nodiscard]] move_info_t move_cursor_up();
    [[nodiscard]] move_info_t move_cursor_down();

    [[nodiscard]] Cursor& get_cursor();
    [[nodiscard]] Cursor get_cursor() const;

    void select_shop_item();
    void deselect_shop_item();
    [[nodiscard]] bn::optional<ItemType> selected_item() const;
    [[nodiscard]] bn::optional<ItemType> specific_item(const index_t index) const;

    bn::optional<ItemType> set_selected_reactor_tile();
    void clear_selected_reactor_tile();
    void swap_selected_reactor_tile_with(const index_t with);
    [[nodiscard]] bn::optional<ItemType> current_tile() const;
    [[nodiscard]] bn::optional<ItemType> specific_tile(const index_t index) const;

    using UpgradePrice_t = Price_t (*)(const status&);
    using UpgradeIcon_t = index_t (*)(const status&);

    index_t get_upgrade_icon(const index_t index, const index_t page) const;
    bn::optional<Price_t> get_upgrade_price(const index_t index, const index_t page) const;

private:
    bool _saved_before{false};
    bool _is_game_started{false};
    bool _tutorial_played{false};
    Cursor _cursor;
    bn::optional<ItemType> _reactor_selected_item;
    bn::array<bn::optional<ItemType>, REACTOR_WIDTH * REACTOR_HEIGHT> _reactor_content;
    bn::array<bn::optional<ItemType>, SHOP_WIDTH * SHOP_HEIGHT> _shop_content;
    using UpgradeBought_t = void (status::*)(const int);
    bn::array<UpgradeBought_t, UPGRADES_WIDTH * UPGRADES_HEIGHT * UPGRADES_PAGES> _upgrades_buy_action;
    bn::array<UpgradePrice_t, UPGRADES_WIDTH * UPGRADES_HEIGHT * UPGRADES_PAGES> _upgrades_get_price;
    bn::array<UpgradeIcon_t, UPGRADES_WIDTH * UPGRADES_HEIGHT * UPGRADES_PAGES> _upgrades_get_icon;
    bn::array<int, UPGRADES_WIDTH * UPGRADES_HEIGHT * UPGRADES_PAGES> _upgrades_level;
    Price_t _money_owned;
};

}

#endif
