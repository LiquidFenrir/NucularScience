/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#ifndef NC_SCENE_H
#define NC_SCENE_H

#include "bn_optional.h"

namespace nc
{

enum class scene_type
{
    INTRO,
    TRANSITION_TO_SPLASH,
    SPLASH_DEV,
    SPLASH_JAM,

    TITLE,
    TITLE_TUTORIAL,
    PLAY_TUTORIAL,

    TRANSITION_TO_GAME,

    REACTOR,
    RESEARCH,
    PREVIEW,
    INDEX,
};

class UpdateResult {

public:
    UpdateResult& operator=(const scene_type);
    bn::optional<scene_type> change_scene() const;
    bool should_skip_transition() const;
    void skip_transition();

private:
    bn::optional<scene_type> _next_scene;
    bool _skip_transition{false};
};

class scene
{

public:
    virtual ~scene() = default;

    [[nodiscard]] virtual UpdateResult update() = 0;

protected:
    scene() = default;
};

}

#endif
