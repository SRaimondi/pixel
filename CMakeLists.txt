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
        tonemapper)

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
        main.cc)

add_executable(pixel ${SOURCE_FILES})