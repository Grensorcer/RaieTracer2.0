#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <math.h>
#include <memory>
#include <numeric>
#include <optional>
#include <tuple>
#include <vector>

#include "blob.hh"
#include "camera.hh"
#include "image.hh"
#include "light.hh"
#include "matrix.hh"
#include "object.hh"
#include "ray.hh"
#include "utils.hh"

using structures::Vec3;
namespace environment
{
    class Scene
    {
    public:
        Scene();
        Scene(Camera &cam)
            : cam_{ cam }
        {}
        Scene(Camera &cam, double ambiant_light)
            : cam_{ cam }
            , ambiant_light_{ ambiant_light }
        {}

        std::optional<environment::intersection_record>
        find_closest_intersection(const Ray &r) const;

        display::Colour compute_light_input(const Vec3 &intersection_point,
                                            const intersection_record &i_r,
                                            int depth) const;

        display::Colour compute_sky(const Ray &r) const;

        display::Colour cast_ray(const Ray &r, int depth) const;

        void add_object(std::shared_ptr<Object> o);
        template <typename Iterator>
        void add_objects(const Iterator &begin, const Iterator &end)
        {
            for (auto i = begin; i != end; ++i)
                objects_.push_back(*i);
        }
        void add_light(std::shared_ptr<Light> l);

    private:
        Camera cam_;
        std::vector<std::shared_ptr<Object>> objects_;
        std::vector<std::shared_ptr<Light>> lights_;
        double ambiant_light_ = 1.;
    };

} // namespace environment
