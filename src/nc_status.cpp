/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_status.h"
#include "bn_sram.h"
#include "bn_cstring.h"

#include "bn_music_items.h"
#include "bn_fixed.h"

namespace nc
{

int ItemType::sprite_index() const
{
    switch(_type)
    {
        case Type::UraniumRod:
            return 1;
        case Type::PlutoniumRod:
            return 2;
        case Type::CaliforniumRod:
            return 3;
        case Type::FantasiumRod:
            return 4;
        case Type::UnobtainiumRod:
            return 5;
        case Type::BlackHoliumRod:
            return 6;
        case Type::BasicVent:
            return 7;
        case Type::SuperVent:
            return 8;
        case Type::BasicPlating:
            return 9;
        case Type::SuperPlating:
            return 10;
        case Type::BasicCoolant:
            return 11;
        case Type::SuperCoolant:
            return 12;
        case Type::BasicAmplifier:
            return 13;
        case Type::SuperAmplifier:
            return 14;
        case Type::BasicBattery:
            return 15;
        case Type::SuperBattery:
            return 16;
        case Type::BlackHoleStabilizer:
            return 17;
        case Type::EnergyExtractor:
            return 18;
        default:
            BN_ERROR("CANNOT HAPPEN");
            return 0; // won't reach
    }
}

ItemType::Type ItemType::from_number(const int value)
{
    return static_cast<ItemType::Type>(value);
}
int ItemType::to_number(const ItemType::Type value)
{
    return static_cast<int>(value);
}

ItemType::ItemType(const Type type) : _type(type)
{

}

ItemType::Type ItemType::get_type() const
{
    return _type;
}

index_t Cursor::index() const
{
    switch(position)
    {
    case Cursor::Position::Shop:
        return row * SHOP_WIDTH + column;
    case Cursor::Position::Reactor:
        return column * REACTOR_HEIGHT + row;
    case Cursor::Position::Upgrades:
        return column * UPGRADES_HEIGHT + row;
    default:
        BN_ERROR("CANNOT HAPPEN");
        return 0; // won't reach
    }
}

status::status()
{
    bn::array<unsigned char, 128> save_data;
    bn::sram::read(save_data);

    if(save_data[0] == 'N')
    {
        // resume saved game
        _saved_before = true;
        _tutorial_played = true;

        _is_game_started = save_data[1];

        bn::memcpy(_upgrades_level.data(), &save_data[6], _upgrades_level.size());

        for(int i = 0; i < _reactor_content.size(); ++i)
        {
            if(const auto val = save_data[i + 30]; val != 0xFF)
            {
                _reactor_content[i].emplace(ItemType::from_number(val));
            }
        }

        bn::memcpy(&_money_owned, &save_data[110], sizeof(_money_owned));
    }
}

void status::initialize()
{
    _upgrades_get_price.fill(nullptr);
    _upgrades_get_icon.fill(nullptr);
    _upgrades_buy_action.fill(nullptr);
    _upgrades_level.fill(0);

    _shop_content.fill(bn::nullopt);
    _reactor_content.fill(bn::nullopt);
    _cursor.row = 0;
    _cursor.column = 0;
    _cursor.position = Cursor::Position::Reactor;
    _shop_content[0].emplace(ItemType::Type::UraniumRod);
}

void status::save(bool game_in_progress)
{
    bn::array<unsigned char, 128> save_data;
    save_data[0] = 'N';
    save_data[1] = game_in_progress ? 0 : 1;
    bn::memcpy(&save_data[6], _upgrades_level.data(), _upgrades_level.size());

    for(int i = 0; i < _reactor_content.size(); ++i)
    {
        if(const auto val = _reactor_content[i]; val)
        {
            save_data[i + 30] = ItemType::to_number(val->get_type());
        }
        else
        {
            save_data[i + 30] = 0xFF;
        }
    }

    bn::memcpy(&_money_owned, &save_data[110], sizeof(_money_owned));
    bn::sram::write(save_data);
}

static bool bgm_playing = false;
void status::start_bgm()
{
    if(!bgm_playing)
    {
        bgm_playing = true;
        bn::music_items::main_theme.play(0.25, true);
    }
}
void stop_bgm()
{
    bn::music_items::main_theme.play(0, false);
}

bool status::have_started_game() const
{
    return _is_game_started;
}
bool status::completed_tutorial() const
{
    return _tutorial_played;
}
void status::mark_tutorial_as_complete()
{
    _tutorial_played = true;
}

void status::start_game()
{
    _is_game_started = true;
    initialize();
}
void status::complete_game()
{
    _is_game_started = false;
}

move_info_t status::move_cursor_left(const bool keep_in_reactor)
{
    switch(_cursor.position)
    {
    case Cursor::Position::Shop:
        if(_cursor.column == 0)
        {
            _cursor.position = Cursor::Position::Reactor;
            _cursor.column = REACTOR_WIDTH - 1;
            return RESET_MOVE;
        }
        else
        {
            _cursor.column -= 1;
            return CURSOR_LEFT_MOVE;
        }
        break;
    case Cursor::Position::Reactor:
        if(_cursor.column == 0)
        {
            if(!keep_in_reactor && _cursor.row < SHOP_HEIGHT)
            {
                _cursor.position = Cursor::Position::Shop;
                _cursor.column = SHOP_WIDTH - 1;
            }
            else
            {
                _cursor.column = REACTOR_WIDTH - 1;
            }
            return RESET_MOVE;
        }
        else
        {
            _cursor.column -= 1;
            return CURSOR_LEFT_MOVE;
        }
        break;
    case Cursor::Position::Upgrades:
        if(_cursor.column == 0)
        {
            _cursor.column = (UPGRADES_WIDTH - 1);
            return RESET_MOVE;
        }
        else
        {
            _cursor.column -= 1;
            return CURSOR_LEFT_MOVE;
        }
        break;
    }

    return 0;
}
move_info_t status::move_cursor_right(const bool keep_in_reactor)
{
    switch(_cursor.position)
    {
    case Cursor::Position::Shop:
        if(_cursor.column == (SHOP_WIDTH - 1))
        {
            _cursor.position = Cursor::Position::Reactor;
            _cursor.column = 0;
            return RESET_MOVE;
        }
        else
        {
            _cursor.column += 1;
            return CURSOR_RIGHT_MOVE;
        }
        break;
    case Cursor::Position::Reactor:
        if(_cursor.column == (REACTOR_WIDTH - 1))
        {
            if(!keep_in_reactor && _cursor.row < SHOP_HEIGHT)
            {
                _cursor.position = Cursor::Position::Shop;
                _cursor.column = 0;
            }
            else
            {
                _cursor.column = 0;
            }
            return RESET_MOVE;
        }
        else
        {
            _cursor.column += 1;
            return CURSOR_RIGHT_MOVE;
        }
        break;
    case Cursor::Position::Upgrades:
        if(_cursor.column == (UPGRADES_WIDTH - 1))
        {
            _cursor.column = 0;
            return RESET_MOVE;
        }
        else
        {
            _cursor.column += 1;
            return CURSOR_RIGHT_MOVE;
        }
        break;
    }

    return 0;
}
move_info_t status::move_cursor_up()
{
    switch(_cursor.position)
    {
    case Cursor::Position::Shop:
        if(_cursor.row == 0)
        {
            _cursor.row = (SHOP_HEIGHT - 1);
            return RESET_MOVE;
        }
        else
        {
            _cursor.row -= 1;
            return CURSOR_UP_MOVE;
        }
        break;
    case Cursor::Position::Reactor:
        if(_cursor.row == 0)
        {
            _cursor.row = (REACTOR_HEIGHT - 1);
            return RESET_MOVE;
        }
        else
        {
            _cursor.row -= 1;
            return CURSOR_UP_MOVE;
        }
        break;
    case Cursor::Position::Upgrades:
        if(_cursor.row == 1)
        {
            _cursor.row = 0;
            return CURSOR_UP_MOVE;
        }
        else
        {
            _cursor.row = 1;
            return CURSOR_DOWN_MOVE;
        }
        break;
    }
    
    return 0;
}
move_info_t status::move_cursor_down()
{
    switch(_cursor.position)
    {
    case Cursor::Position::Shop:
        if(_cursor.row == (SHOP_HEIGHT - 1))
        {
            _cursor.row = 0;
            return RESET_MOVE;
        }
        else
        {
            _cursor.row += 1;
            return CURSOR_DOWN_MOVE;
        }
        break;
    case Cursor::Position::Reactor:
        if(_cursor.row == (REACTOR_HEIGHT - 1))
        {
            _cursor.row = 0;
            return RESET_MOVE;
        }
        else
        {
            _cursor.row += 1;
            return CURSOR_DOWN_MOVE;
        }
        break;
    case Cursor::Position::Upgrades:
        if(_cursor.row == 1)
        {
            _cursor.row = 0;
            return CURSOR_UP_MOVE;
        }
        else
        {
            _cursor.row = 1;
            return CURSOR_DOWN_MOVE;
        }
        break;
    }
    
    return 0;
}

Cursor& status::get_cursor()
{
    return _cursor;
}

Cursor status::get_cursor() const
{
    return _cursor;
}

void status::select_shop_item()
{
    if(_cursor.position == Cursor::Position::Shop)
    {
        _reactor_selected_item = _shop_content[_cursor.index()];
    }
}
void status::deselect_shop_item()
{
    _reactor_selected_item.reset();
}
bn::optional<ItemType> status::selected_item() const
{
    return _reactor_selected_item;
}
bn::optional<ItemType> status::specific_item(const index_t index) const
{
    return _shop_content[index];
}

bn::optional<ItemType> status::set_selected_reactor_tile()
{
    if(_cursor.position == Cursor::Position::Reactor && _reactor_selected_item)
        return (_reactor_content[_cursor.index()] = _reactor_selected_item);
    return bn::nullopt;
}
void status::clear_selected_reactor_tile()
{
    if(_cursor.position == Cursor::Position::Reactor)
        _reactor_content[_cursor.index()].reset();
}
void status::swap_selected_reactor_tile_with(const index_t with)
{
    if(_cursor.position != Cursor::Position::Reactor)
        return;

    const auto current_index = _cursor.index();
    if(with == current_index)
        return;
    
    bn::swap(_reactor_content[with], _reactor_content[current_index]);
}
bn::optional<ItemType> status::current_tile() const
{
    if(_cursor.position == Cursor::Position::Reactor)
        return specific_tile(_cursor.index());

    return bn::nullopt;
}
bn::optional<ItemType> status::specific_tile(const index_t index) const
{
    return _reactor_content[index];
}

index_t status::get_upgrade_icon(const index_t index, const index_t page) const
{
    const auto func = _upgrades_get_icon[index + page * (UPGRADES_WIDTH * UPGRADES_HEIGHT)];
    if(func) return func(*this);
    else return 0;
}

bn::optional<Price_t> status::get_upgrade_price(const index_t index, const index_t page) const
{
    const auto func = _upgrades_get_price[index + page * (UPGRADES_WIDTH * UPGRADES_HEIGHT)];
    if(func) return func(*this);
    else return bn::nullopt;
}

}
