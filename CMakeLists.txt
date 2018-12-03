cmake_minimum_required(VERSION 3.12)
project(raytracer)

set(CMAKE_CXX_STANDARD 98)

add_executable(raytracer src/main.cpp src/vec3.h src/ray.h src/hitable.h src/sphere.h src/hitable_list.h src/camera.h src/material.h src/lambertian.h src/metal.h src/dielectric.h src/aabb.h src/bvh_node.h src/texture.h src/perlin.h src/stb_image.h)