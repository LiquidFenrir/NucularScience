/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_TUTORIAL_H
#define NC_TUTORIAL_H

#include "bn_optional.h"
#include "bn_array.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_string_view.h"
#include "bn_blending_actions.h"
#include "bn_sprite_actions.h"

#include "nc_scene.h"
#include "nc_status_fwd.h"

namespace bn
{
    class sprite_text_generator;
}

namespace nc
{

struct TutorialPage {
    bn::string_view title;
    bn::array<bn::string_view, 3> lines;
};

class tutorial : public scene
{

public:
    tutorial(nc::status& status, const scene_type after, bn::sprite_text_generator& text_generator);

    [[nodiscard]] UpdateResult update() final;

private:
    nc::status& _status;
    const scene_type _after;
    bn::sprite_text_generator& _text_generator;
    bn::vector<bn::sprite_ptr, 4> _title_sprites;
    bn::vector<bn::sprite_ptr, 32> _info_sprites;
    bn::optional<bn::blending_transparency_alpha_to_action> _text_blend_action;
    int _page_index;
    bool _going_in;
    bool _going_out;

    void _clear_text();
    void _set_text(const TutorialPage& page);
};

}

#endif
