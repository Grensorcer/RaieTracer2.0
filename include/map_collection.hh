#pragma once
#include "scene_collection.hh"

namespace environment
{
    constexpr maps paper_wrinkled =
        std::make_tuple<>("../data/maps/Paper_Wrinkled_001_basecolor.jpg",
                          "../data/maps/Paper_Wrinkled_001_normal.jpg",
                          "../data/maps/Paper_Wrinkled_001_height.png");
    constexpr maps metal_gate =
        std::make_tuple<>("../data/maps/Metal_Gate_002_basecolor.jpg",
                          "../data/maps/Metal_Gate_002_normal.jpg",
                          "../data/maps/Metal_Gate_002_height.png");
    constexpr maps catacomb_wall =
        std::make_tuple<>("../data/maps/Catacomb_Wall_001_basecolor.jpg",
                          "../data/maps/Catacomb_Wall_001_normal.jpg",
                          "../data/maps/Catacomb_Wall_001_height.png");
    constexpr maps wood_wall =
        std::make_tuple<>("../data/maps/Wood_Wall_003_basecolor.jpg",
                          "../data/maps/Wood_Wall_003_normal.jpg",
                          "../data/maps/Wood_Wall_003_height.png");
    constexpr maps ground_skull =
        std::make_tuple<>("../data/maps/Ground Skull_basecolor.jpg",
                          "../data/maps/Ground Skull_normal.jpg",
                          "../data/maps/Ground Skull_height.png");
    constexpr maps seamless_pebbles =
        std::make_tuple<>("../data/maps/Seamless_Pebbles_Texture.jpg",
                          "../data/maps/Seamless_Pebbles_Texture_NORMAL.jpg",
                          "../data/maps/Seamless_Pebbles_Texture_DISP.png");

} // namespace environment
