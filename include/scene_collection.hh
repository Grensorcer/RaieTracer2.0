#pragma once
#include "scene.hh"

namespace environment
{
    Scene &scene_one(Scene &s);
    Scene &scene_triangles(Scene &s);
    Scene &scene_smooth_triangles(Scene &s);
    Scene &scene_triangle_reflect(Scene &s);
    Scene &scene_sphere_reflect(Scene &s);
    Scene &scene_sphere_triangle_reflect(Scene &s);
    Scene &scene_blob(Scene &s);
    Scene &scene_mesh(Scene &s);
    Scene &scene_sphere_texture_reflect(Scene &s);
} // namespace environment
