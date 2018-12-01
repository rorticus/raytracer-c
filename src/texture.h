//
// Created by Rory Mulligan on 2018-11-30.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H


#include "vec3.h"
#include "perlin.h"

class texture {
public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class constant_texture : public texture {
public:
    constant_texture() {}

    constant_texture(vec3 c) : color(c) {}

    vec3 value(float u, float v, const vec3 &p) const override {
        return color;
    }

    vec3 color;
};

class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}

    vec3 value(float u, float v, const vec3 &p) const override {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }

    texture *odd;
    texture *even;
};

class noise_texture : public texture {
public:
    noise_texture() {}

    vec3 value(float u, float v, const vec3 &p) const override {
        return vec3(1, 1, 1) * noise.noise(p);
    }

    perlin noise;
};


#endif //RAYTRACER_TEXTURE_H
