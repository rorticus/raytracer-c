//
// Created by Rory Mulligan on 2018-11-30.
//

#ifndef RAYTRACER_AABB_H
#define RAYTRACER_AABB_H

#include "vec3.h"
#include "ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }

inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
public:
    aabb() {}

    aabb(const vec3 a, const vec3 b) {
        _min = a;
        _max = b;
    }

    vec3 min() const { return _min; }

    vec3 max() const { return _max; }

    bool hit(const ray &r, float tmin, float tmax) const {
        for (int a = 0; a < 3; a++) {
            float minA = (_min[a] - r.origin()[a]) / r.direction()[a];
            float maxA = (_max[a] - r.origin()[a]) / r.direction()[a];
            float t0 = ffmin(minA, maxA);
            float t1 = ffmax(minA, maxA);

            tmin = ffmax(t0, tmin);
            tmax = ffmax(t1, tmax);

            if (tmax <= tmin) {
                return false;
            }
        }

        return true;
    }

    vec3 _min;
    vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small(ffmax(box0.max().x(), box1.max().x()),
               ffmax(box0.max().y(), box1.max().y()),
               ffmax(box0.max().z(), box1.max().z()));
    vec3 big(ffmax(box0.max().x(), box1.max().x()),
             ffmax(box0.max().y(), box1.max().y()),
             ffmax(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}

#endif //RAYTRACER_AABB_H
