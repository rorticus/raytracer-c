//
// Created by Rory Mulligan on 11/25/18.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "hitable.h"

class sphere : public hitable {
public:
    sphere() = default;

    sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), m(mat) {};

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

    vec3 center;
    float radius{};
    material *m;
};

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = -0.5f * b / a;
    else {
        float q = (b > 0) ?
                  -0.5f * (b + sqrt(discr)) :
                  -0.5f * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    float t1, t0;

    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return false;
    }

    if (t0 < t_max && t0 > t_min) {
        rec.t = t0;
        rec.p = r.pointAtParameter(t0);
        rec.normal = unit_vector(rec.p - center);
        rec.mat_ptr = m;
        return true;
    }

    return false;
}

class moving_sphere : public hitable {
public:
    moving_sphere() {}

    moving_sphere(vec3 cen0, vec3 cen1, float r, float t0, float t1, material *mat) : center0(cen0), center1(cen1),
                                                                                      radius(r), time0(t0), time1(t1),
                                                                                      m(mat) {};
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;
    vec3 center(float t) const;

    vec3 center0, center1;
    float time0, time1;
    float radius;
    material *m;
};

vec3 moving_sphere::center(float t) const {
    return center0 + ((t - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 cent = center(r.time());

    vec3 oc = r.origin() - cent;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    float t1, t0;

    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return false;
    }

    if (t0 < t_max && t0 > t_min) {
        rec.t = t0;
        rec.p = r.pointAtParameter(t0);
        rec.normal = unit_vector(rec.p - cent);
        rec.mat_ptr = m;
        return true;
    }

    return false;
}

#endif //RAYTRACER_SPHERE_H
