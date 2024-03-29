#pragma once
#include "scene_collection.hh"

namespace environment
{
    constexpr model froggy =
        std::make_pair<>("../data/models/froggy.stl", 1.862);
    constexpr model new_froggy =
        std::make_pair<>("../data/models/new_froggy.stl", 1.431);
    constexpr model teapot = std::make_pair<>("../data/models/teapot.stl", 2.2);
    constexpr model lion = std::make_pair<>("../data/models/lion.stl", 0.9);
    constexpr model wall1 = std::make_pair<>("../data/models/wall1.stl", 2.);
    constexpr model wall2 = std::make_pair<>("../data/models/wall2.stl", 2.);
} // namespace environment
