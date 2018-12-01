//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H


#include "material.h"

class metal : public material {
public:
    explicit metal(const vec3 &a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

    bool scatter(const ray &ray_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    };

    vec3 albedo;
    float fuzz;
};


#endif //RAYTRACER_METAL_H
