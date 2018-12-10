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
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "diffuse_light.h"
#include "xy_rect.h"
#include "yz_rect.h"
#include "xz_rect.h"
#include "flip_normals.h"

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

hitable *cornell_box() {
    hitable **list = new hitable*[8];
    int i = 0;
    material *red = new lambertian( new constant_texture(vec3(0.65, 0.05, 0.05)) );
    material *white = new lambertian( new constant_texture(vec3(0.73, 0.73, 0.73)) );
    material *green = new lambertian( new constant_texture(vec3(0.12, 0.45, 0.15)) );
    material *light = new diffuse_light( new constant_texture(vec3(15, 15, 15)) );
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    return new hitable_list(list, i);
}

vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;

    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation * color(scattered, world, depth + 1);
        } else {
            return emitted;
        }
    } else {
        return vec3(0, 0, 0);
    }
}

int main() {
    int nx = 250;
    int ny = 250;
    int ns = 100;

    std::ofstream output;
    output.open("out.ppm");

    output << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *world = cornell_box();

    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    float dist_to_focus = 10.0;
    float aperature = 0;
    float vfov = 40;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperature, dist_to_focus, 0, 1);

    for (int j = ny - 1; j >= 0; j--) {
        std::cout << ny - j << " / " << ny << "\n";
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

            output << ir << " " << ig << " " << ib << "\n";
        }
    }

    output.close();

    return 0;
}

