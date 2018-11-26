//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H


#include "vec3.h"
#include "ray.h"

class material;

struct hit_record {
    float t{};
    vec3 p;
    vec3 normal;
    material *mat_ptr{};
};

class hitable {
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
