//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include "vec3.h"

class ray {
public:
    ray() = default;

    ray(const vec3 &a, const vec3 &b) {
        A = a;
        B = b;
    }

    vec3 origin() const { return A; }

    vec3 direction() const { return B; }

    vec3 pointAtParameter(float t) const { return A + t * B; }

    vec3 A;
    vec3 B;
};


#endif //RAYTRACER_RAY_H
