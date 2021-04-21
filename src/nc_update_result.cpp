/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "nc_scene.h"

namespace nc
{

UpdateResult& UpdateResult::operator=(const scene_type next_scene)
{
    _next_scene = next_scene;
    return *this;
}

bn::optional<scene_type> UpdateResult::change_scene() const
{
    return _next_scene;
}
bool UpdateResult::should_skip_transition() const
{
    return _skip_transition;
}
void UpdateResult::skip_transition()
{
    _skip_transition = true;
}

}
