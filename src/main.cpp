/*
 * Copyright (c) 2021 LiquidFenrir
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_sprite_text_generator.h"
#include "bn_optional.h"
#include "bn_memory.h"
#include "bn_bg_palettes.h"

#include "variable_8x16_sprite_font.h"

#include "nc_status.h"
#include "nc_scene.h"

#include "nc_intro.h"
#include "nc_transition.h"
#include "nc_splash.h"
#include "nc_title.h"
#include "nc_tutorial.h"
#include "nc_reactor_screen.h"
#include "nc_research_screen.h"
#include "nc_preview_screen.h"
#include "nc_index_screen.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    text_generator.set_bg_priority(1);

    bn::bg_palettes::set_transparent_color(bn::color(14, 0, 0));
    bn::unique_ptr<nc::scene> scene(new nc::intro(text_generator));
    nc::UpdateResult next_scene;
    int wait_frames{0};

    nc::status status;

    while(true)
    {
        if(scene)
        {
            next_scene = scene->update();
        }

        bn::core::update();

        if(const auto next = next_scene.change_scene(); next)
        {
            if(scene)
            {
                if(next_scene.should_skip_transition())
                {
                    wait_frames = 1;
                }
                else
                {
                    scene.reset();
                    wait_frames = 15;
                }
            }

            --wait_frames;

            if(! wait_frames)
            {
                switch(*next)
                {

                case nc::scene_type::INTRO:
                    scene.reset(new nc::intro(text_generator));
                    break;
                case nc::scene_type::TRANSITION_TO_SPLASH:
                    scene.reset(new nc::transition(bn::color(23, 21, 27), nc::scene_type::SPLASH));
                    break;
                case nc::scene_type::SPLASH:
                    scene.reset(new nc::splash);
                    break;
                case nc::scene_type::TITLE:
                    scene.reset(new nc::title(status, text_generator));
                    break;
                case nc::scene_type::TITLE_TUTORIAL:
                    scene.reset(new nc::tutorial(status, nc::scene_type::TITLE, text_generator));
                    break;
                case nc::scene_type::PLAY_TUTORIAL:
                    scene.reset(new nc::tutorial(status, nc::scene_type::TRANSITION_TO_GAME, text_generator));
                    break;

                case nc::scene_type::TRANSITION_TO_GAME:
                    scene.reset(new nc::transition(bn::color(16, 16, 16), nc::scene_type::REACTOR));
                    break;

                case nc::scene_type::REACTOR:
                    scene.reset(new nc::reactor_screen(status, text_generator));
                    break;
                case nc::scene_type::RESEARCH:
                    scene.reset(new nc::research_screen(status, text_generator));
                    break;
                case nc::scene_type::PREVIEW:
                    scene.reset(new nc::preview_screen(status, text_generator));
                    break;
                case nc::scene_type::INDEX:
                    scene.reset(new nc::index_screen(status, text_generator));
                    break;

                default:
                    BN_ERROR("Invalid next scene: ", int(*next));
                    break;
                }
            }
        }
    }

/*
    while(true)
    {
        regular_bgs_visibility_scene(text_generator);
        bn::core::update();

        regular_bgs_visibility_actions_scene(text_generator);
        bn::core::update();

        regular_bgs_position_scene(text_generator);
        bn::core::update();

        regular_bgs_position_actions_scene(text_generator);
        bn::core::update();

        regular_bgs_position_hbe_scene(text_generator);
        bn::core::update();

        regular_bgs_priority_scene(text_generator);
        bn::core::update();

        regular_bgs_z_order_scene(text_generator);
        bn::core::update();

        regular_bgs_put_above_scene(text_generator);
        bn::core::update();

        regular_bgs_attributes_scene(text_generator);
        bn::core::update();

        regular_bgs_attributes_hbe_scene(text_generator);
        bn::core::update();

        regular_bg_builder_scene(text_generator);
        bn::core::update();
    }
*/
}
