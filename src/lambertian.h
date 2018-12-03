//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H


#include "material.h"
#include "texture.h"

class lambertian : public material {
public:
    explicit lambertian(texture *a) : albedo(a) {}

    bool scatter(const ray &ray_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, ray_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    };

    texture *albedo;
};


#endif //RAYTRACER_LAMBERTIAN_H
