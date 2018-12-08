//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include "ray.h"
#include "hitable.h"

class material {
public:
    virtual bool scatter(const ray &ray_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;

    virtual vec3 emitted(float u, float v, const vec3 &p) const {
        return vec3(0, 0, 0);
    }
};


#endif //RAYTRACER_MATERIAL_H
