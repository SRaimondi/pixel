cmake_minimum_required(VERSION 3.6)
project(pixel)

# Set verbose build
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set general flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -m64 -march=native")

# Set DEBUG flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -DDEBUG")

# Set RELEASE flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Include directories
include_directories(camera
        core
        film
        integrator
        material
        primitives
        renderer
        shapes
        tonemapper
        lights
        texture)

set(SOURCE_FILES
        camera/pinhole_camera.cc
        camera/pinhole_camera.h
        core/bbox.cc
        core/bbox.h
        core/camera.h
        core/film.cc
        core/film.h
        core/integrator.cc
        core/integrator.h
        core/interaction.cc
        core/interaction.h
        core/material.h
        core/montecarlo.cc
        core/montecarlo.h
        core/pixel.h
        core/primitive.h
        core/ray.cc
        core/ray.h
        core/renderer.cc
        core/renderer.h
        core/scattering.cc
        core/scattering.h
        core/scene.cc
        core/scene.h
        core/shape.cc
        core/shape.h
        core/sse_matrix.cc
        core/sse_matrix.h
        core/sse_spectrum.h
        core/sse_vector.h
        core/tonemapper.cc
        core/tonemapper.h
        core/transform.cc
        core/transform.h
        film/box_film.cc
        film/box_film.h
        integrator/debug_integrator.cc
        integrator/debug_integrator.h
        material/emitting_material.cc
        material/emitting_material.h
        material/matte_material.cc
        material/matte_material.h
        primitives/instance.cc
        primitives/instance.h
        primitives/prim_list.cc
        primitives/prim_list.h
        renderer/sampler_renderer.cc
        renderer/sampler_renderer.h
        shapes/rectangle.cc
        shapes/rectangle.h
        shapes/sphere.cc
        shapes/sphere.h
        tonemapper/clamp_tonemapper.cc
        tonemapper/clamp_tonemapper.h
        main.cc
        core/light.h
        core/light.cpp
        lights/point_light.h
        lights/point_light.cpp
        integrator/direct_integrator.h
        integrator/direct_integrator.cpp
        lights/area_light.h
        lights/area_light.cpp
        material/mirror_material.h
        material/mirror_material.cpp
        integrator/whitted_integrator.h
        integrator/whitted_integrator.cpp
        material/glass_material.h
        material/glass_material.cpp
        integrator/path_tracer_integrator.h
        integrator/path_tracer_integrator.cpp
        core/texture.h
        texture/constant_texture.h
        texture/checkboard_texture.h
        # texture/grid_texture.h
        core/texture.cpp)

add_executable(pixel ${SOURCE_FILES})