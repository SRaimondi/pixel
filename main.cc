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
#include "shape_list.h"
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

int main(int argc, char **argv) {

    // Create film
    pixel::Film *f = new pixel::BoxFilterFilm(1024, 1024);

    // Create camera
    pixel::CameraInterface *camera = new pixel::PinholeCamera(
            pixel::SSEVector(-15.f, 20.f, 20.f, 1.f), pixel::SSEVector(0.f, 0.f, 0.f, 1.f),
            pixel::SSEVector(0.f, 1.f, 0.f, 0.f),
            60.f, f->GetWidth(), f->GetHeight());

    pixel::PrimitiveList list;

    pixel::ShapeInterface *s = new pixel::Sphere(pixel::Translate(-4.f, 2.f, 0.f), 2.f);
    pixel::PrimitiveInterface *p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.9f, 0.f, 0.f)));
    list.AddPrimitive(p);

    s = new pixel::Sphere(pixel::Translate(4.f, 2.f, 0.f), 2.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.9f, 0.f)));
    list.AddPrimitive(p);

    s = new pixel::Sphere(pixel::Translate(0.f, 2.f, 3.f), 2.f);
    //p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.9f, 0.9f, 0.f)));
    p = new pixel::Instance(s, new pixel::GlassMaterial(pixel::SSESpectrum(0.8f), pixel::SSESpectrum(0.8f), 1.3f));
    list.AddPrimitive(p);

    s = new pixel::Sphere(pixel::Translate(0.f, 2.f, -3.f), 2.f);
    //p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.1f, 0.9f)));
    p = new pixel::Instance(s, new pixel::MirrorMaterial(pixel::SSESpectrum(0.9f)));
    list.AddPrimitive(p);


//    pixel::Ray r = pixel::Ray(pixel::SSEVector(0.f, 5.f, 0.f, 1.f), pixel::SSEVector(0.f, -1.f, 0.f, 0.f));
//    pixel::SurfaceInteraction interaction;
//    float thit;
//    bool hit = s->Intersect(r, &thit, &interaction);

//    s = new pixel::Sphere(pixel::SSEVector(0.f, 2.f, 0.f, 1.f), 2.f);
//    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.9f, 0.f)), pixel::SSEMatrix());
//    //list.AddPrimitive(p);
//
//    s = new pixel::Sphere(pixel::SSEVector(-2.f, 6.f, 0.f, 1.f), 2.f);
//    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.f, 0.9f)), pixel::SSEMatrix());
//    //list.AddPrimitive(p);
//
//    s = new pixel::Sphere(pixel::SSEVector(2.f, 6.f, 0.f, 1.f), 2.f);
//    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.9f, 0.9f, 0.f)), pixel::SSEMatrix());
//    //list.AddPrimitive(p);

    s = new pixel::Rectangle(pixel::SSEMatrix(), 20.f, 20.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.1f, 0.9f, 0.9f)));
    //p = new pixel::Instance(s, new pixel::MirrorMaterial(pixel::SSESpectrum(0.9f)));
    list.AddPrimitive(p);


    s = new pixel::Rectangle(pixel::Translate(0.f, 10.f, 0.f) * pixel::RotateX(180.f), 5.f, 5.f);
    pixel::AreaLight *area_light_r = new pixel::AreaLight(s, new pixel::EmittingMaterial(pixel::SSESpectrum(10.f)));
    //list.AddPrimitive(area_light_r);

    s = new pixel::Sphere(pixel::Translate(0.f, 10.f, 0.f), 2.5f);
    pixel::AreaLight *area_light_s = new pixel::AreaLight(s, new pixel::EmittingMaterial(pixel::SSESpectrum(10.f)));
    list.AddPrimitive(area_light_s);

    // Create scene
    pixel::Scene scene(&list);

    // Add light
    //scene.AddLight(new pixel::PointLight(pixel::SSEVector(0.f, 5.f, 8.f, 1.f), pixel::SSESpectrum(100.f)));
    //scene.AddLight(new pixel::PointLight(pixel::SSEVector(0.f, 5.f, -8.f, 1.f), pixel::SSESpectrum(100.f)));
    //scene.AddLight(area_light_r);
    scene.AddLight(area_light_s);

    // Create renderer
//    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
//            new pixel::DebugIntegrator(pixel::DebugMode::DEBUB_HIT), 1);
    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
            new pixel::WhittedIntegrator(), 16);
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