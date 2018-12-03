#include <iostream>
#include <stdio.h>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "bvh_node.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable *[n + 1];
    texture *noise = new noise_texture(3);

    int nx, ny, nn;
    unsigned char *texture_data = stbi_load("../assets/2k_venus_surface.jpg", &nx, &ny, &nn, 0);
    material *tex = new lambertian(new image_texture(texture_data, nx, ny));

    list[0] = new sphere(vec3(0, -1000.0f, 0), 1000, new lambertian(noise));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    list[i++] = new moving_sphere(center, center + vec3(0, float(0.5f * drand48()), 0), 0.2, 0.0, 1.0,
                                                  new lambertian(new constant_texture(
                                                          vec3(drand48() * drand48(), drand48() * drand48(),
                                                               drand48() * drand48()))));
                } else if (choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2, new metal(
                            vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())),
                            0.5 * drand48()));
                } else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, tex);

    bvh_node *bnode = new bvh_node(list, i, 0.0f, 1.0f);

    return bnode;
}

vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;

    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return vec3(0, 0, 0);
        }
    } else {
        vec3 unit = unit_vector(r.direction());
        float t = 0.5f * (unit.y() + 1.0f);
        return (1.0 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

int main() {
    int nx = 250;
    int ny = 250;
    int ns = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *world = random_scene();

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperature = 0.25;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperature, dist_to_focus, 0, 1);

    for (int j = ny - 1; j >= 0; j--) {
        fprintf(stderr, "%d / %d\n", ny - j, ny);
        for (int i = 0; i < nx; i++) {
            vec3 col = vec3(0, 0, 0);

            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                ray r = cam.get_ray(u, v);

                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}

