//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H


#include "material.h"

float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * (float) pow(1 - cosine, 5);
}

class dielectric : public material {
public:
    explicit dielectric(float ri) : ref_idx(ri) {}

    bool scatter(const ray &ray_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(ray_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(ray_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(ray_in.direction(), rec.normal) / ray_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(ray_in.direction(), rec.normal) / ray_in.direction().length();
        }

        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            scattered = ray(rec.p, reflected, ray_in.time());
            reflect_prob = 1.0;
        }

        if (drand48() < reflect_prob) {
            scattered = ray(rec.p, reflected, ray_in.time());
        } else {
            scattered = ray(rec.p, refracted, ray_in.time());
        }

        return true;
    };

    float ref_idx;
};


#endif //RAYTRACER_DIELECTRIC_H
