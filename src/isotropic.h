//
// Created by Rory Mulligan on 2018-12-09.
//

#ifndef RAYTRACER_ISOTROPIC_H
#define RAYTRACER_ISOTROPIC_H


#include "material.h"
#include "texture.h"

class isotropic : public material {
public:
    isotropic(texture *a) : albedo(a) {}

    virtual bool scatter(const ray &ray_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        scattered = ray(rec.p, random_in_unit_sphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    texture *albedo;
};


#endif //RAYTRACER_ISOTROPIC_H
