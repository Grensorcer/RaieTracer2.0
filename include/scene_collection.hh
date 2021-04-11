#pragma once
#include "scene.hh"

namespace environment
{
    using maps = std::tuple<char const *, char const *, char const *>;
    using model = std::pair<const char *const, double>;
    Scene &scene_one(Scene &s);
    Scene &scene_triangles(Scene &s);
    Scene &scene_smooth_triangles(Scene &s);
    Scene &scene_triangle_reflect(Scene &s);
    Scene &scene_sphere_reflect(Scene &s);
    Scene &scene_sphere_triangle_reflect(Scene &s);
    Scene &scene_blob(Scene &s, const maps &m);
    Scene &scene_mesh(Scene &s, const maps &m, const model &mod);
    Scene &scene_sphere_texture_reflect(Scene &s, const maps &m);
    Scene &scene_sphere_texture_reflect_height(Scene &s, const maps &m);
} // namespace environment
