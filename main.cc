/*
 * The MIT License
 *
 * Copyright 2016 simon.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


/* 
 * File:   main.cc
 * Author: simon
 *
 * Created on October 21, 2016, 3:46 PM
 */

#include <cstdlib>
#include "box_film.h"
#include "clamp_tonemapper.h"
#include "ray.h"
#include "matrix.h"
#include "camera.h"
#include "pinhole_camera.h"
#include "scene.h"
#include "integrator.h"
#include "sphere.h"
#include "renderer.h"
#include "sampler_renderer.h"
#include "debug_integrator.h"
#include "direct_integrator.h"
#include "path_tracer_integrator.h"
#include "whitted_integrator.h"
#include "interaction.h"
#include "instance.h"
#include "transform.h"
#include "rectangle.h"
#include "matte_material.h"
#include "emitting_material.h"
#include "mirror_material.h"
#include "glass_material.h"
#include "prim_list.h"
#include "point_light.h"
#include "area_light.h"
#include "constant_texture.h"
#include "checkboard_texture.h"
#include "grid_texture.h"

int main(int argc, char **argv) {

    // Create film
    pixel::Film *f = new pixel::BoxFilterFilm(1024, 1024);

    // Create camera
    pixel::CameraInterface *camera = new pixel::PinholeCamera(
            pixel::SSEVector(-15.f, 20.f, 20.f, 1.f), pixel::SSEVector(0.f, 0.f, 0.f, 1.f),
            pixel::SSEVector(0.f, 1.f, 0.f, 0.f),
            60.f, f->GetWidth(), f->GetHeight());

    pixel::PrimitiveList list;

    // Create textures
    auto white_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.9f));
    auto black_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.05f));
    auto red_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.9f, 0.f, 0.f));
    auto green_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.f, 0.9f, 0.f));
    auto green2_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.1f, 0.9f, 0.9f));
    auto violet_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(
            pixel::SSESpectrum(0.8f, 0.f, 0.8f));
    auto sigma_tex = std::make_shared<const pixel::ConstantTexture<float>>(0.f);
    auto ref_tex = std::make_shared<const pixel::ConstantTexture<float>>(1.3f);
    auto mirror_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(pixel::SSESpectrum(0.9f));
    auto emission_tex = std::make_shared<const pixel::ConstantTexture<pixel::SSESpectrum>>(pixel::SSESpectrum(10.f));

    // Create texture mapping
    auto uv_map = std::make_shared<const pixel::UVMapping2D>(6.f, 6.f, 0.f, 0.f);
    auto checkboard_tex = std::make_shared<const pixel::CheckboardTexture<pixel::SSESpectrum>>(uv_map, emission_tex,
                                                                                               black_tex);
    auto grid_tex = std::make_shared<const pixel::GridTexture<pixel::SSESpectrum>>(uv_map, green2_tex, violet_tex,
                                                                                   0.07f);

    // Create shapes
    auto s1 = std::make_shared<const pixel::Sphere>(pixel::Translate(-4.f, 2.f, 0.f), 2.f);
    auto m1 = std::make_shared<const pixel::MatteMaterial>(red_tex, sigma_tex);
    auto p1 = std::make_shared<const pixel::Instance>(s1, m1);
    list.AddPrimitive(p1.get());

    auto s2 = std::make_shared<const pixel::Sphere>(pixel::Translate(4.f, 2.f, 0.f), 2.f);
    auto m2 = std::make_shared<const pixel::MatteMaterial>(green_tex, sigma_tex);
    auto p2 = std::make_shared<const pixel::Instance>(s2, m2);
    list.AddPrimitive(p2.get());

    auto s3 = std::make_shared<const pixel::Sphere>(pixel::Translate(0.f, 2.f, 3.f), 2.f);
    auto g1 = std::make_shared<const pixel::GlassMaterial>(mirror_tex, mirror_tex, ref_tex);
    auto p3 = std::make_shared<const pixel::Instance>(s3, g1);
    list.AddPrimitive(p3.get());

    auto s4 = std::make_shared<const pixel::Sphere>(pixel::Translate(0.f, 2.f, -4.f) * pixel::Scale(3.f, 1.f, 1.f),
                                                    2.f);
    auto mirror1 = std::make_shared<const pixel::MirrorMaterial>(mirror_tex);
    auto p4 = std::make_shared<const pixel::Instance>(s4, mirror1);
    list.AddPrimitive(p4.get());

    auto r = std::make_shared<const pixel::Rectangle>(pixel::SSEMatrix(), 20.f, 20.f);
    auto m3 = std::make_shared<const pixel::MatteMaterial>(grid_tex, sigma_tex);
    auto p5 = std::make_shared<const pixel::Instance>(r, m3);
    list.AddPrimitive(p5.get());

    auto sphere_light = std::make_shared<const pixel::Sphere>(pixel::Translate(0.f, 10.f, 0.f), 2.5f);
    auto rectangle_light = std::make_shared<const pixel::Rectangle>(pixel::Translate(0.f, 10.f, 0.f) * pixel::RotateX(180.f), 10.f, 10.f);
    auto emitting_mat = std::make_shared<const pixel::EmittingMaterial>(emission_tex);
    auto area_light = std::make_shared<const pixel::AreaLight>(sphere_light, emitting_mat);
    list.AddPrimitive(area_light.get());

    // Create scene
    pixel::Scene scene(&list);

    // Add light
    scene.AddLight(area_light.get());

    // Create renderer
//    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
//            new pixel::DebugIntegrator(pixel::DebugMode::DEBUG_NORMAL), 1);
    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
            new pixel::WhittedIntegrator(), 64);
//    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
//            new pixel::PathTracerIntegrator(), 512
//    );

    // Render image
    renderer->RenderImage(f, scene, *camera);

    // Create tone mapper
    pixel::ToneMapperInterface *t = new pixel::ClampToneMapper(1.f);
    // Process image and create it
    t->Process(std::string("test.ppm"), *f);

    return 0;
}
