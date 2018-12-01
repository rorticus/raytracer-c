//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include "vec3.h"

class ray {
public:
    ray() {};

    ray(const vec3 &a, const vec3 &b, float ti = 0.0f) {
        A = a;
        B = b;
        t = ti;
    }

    vec3 origin() const { return A; }

    vec3 direction() const { return B; }

    vec3 pointAtParameter(float t) const { return A + t * B; }

    float time() const { return t; }

    vec3 A;
    vec3 B;
    float t;
};


#endif //RAYTRACER_RAY_H
