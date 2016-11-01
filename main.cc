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
#include "interaction.h"
#include "shape_list.h"
#include "instance.h"
#include "transform.h"
#include "rectangle.h"
#include "matte_material.h"
#include "prim_list.h"

int main(int argc, char **argv) {

    // Create film
    pixel::Film *f = new pixel::BoxFilterFilm(1024, 1024);

    // Create camera
    pixel::CameraInterface *camera = new pixel::PinholeCamera(
            pixel::SSEVector(0.f, 5.f, 20.f, 1.f), pixel::SSEVector(0.f, 0.f, 0.f, 1.f),
            pixel::SSEVector(0.f, 1.f, 0.f, 0.f),
            60.f, f->GetWidth(), f->GetHeight());

    // Create sphere
    //pixel::ShapeList list;

    pixel::PrimitiveList list;

    pixel::ShapeInterface *s = new pixel::Sphere(pixel::SSEVector(2.f, 2.f, 0.f, 1.f), 2.f);
    pixel::PrimitiveInterface *p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.9f, 0.f, 0.f)),
                                                       pixel::SSEMatrix());
    list.AddPrimitive(p);
    //list.AddShape(s);
    s = new pixel::Sphere(pixel::SSEVector(-2.f, 2.f, 0.f, 1.f), 2.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.9f, 0.f)), pixel::SSEMatrix());
    list.AddPrimitive(p);
    //list.AddShape(s);
    s = new pixel::Sphere(pixel::SSEVector(-2.f, 6.f, 0.f, 1.f), 2.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.f, 0.9f)), pixel::SSEMatrix());
    list.AddPrimitive(p);
    //list.AddShape(s);
    s = new pixel::Sphere(pixel::SSEVector(2.f, 6.f, 0.f, 1.f), 2.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.9f, 0.9f, 0.f)), pixel::SSEMatrix());
    list.AddPrimitive(p);
    //list.AddShape(s);
    s = new pixel::Rectangle(10.f, 10.f);
    p = new pixel::Instance(s, new pixel::MatteMaterial(pixel::SSESpectrum(0.f, 0.9f, 0.9f)), pixel::SSEMatrix());
    list.AddPrimitive(p);
    //list.AddShape(s);


    // Create scene
    pixel::Scene scene(&list);

    // Create renderer
    pixel::RendererInterface *renderer = new pixel::SamplerRenderer(
            new pixel::DebugIntegrator(pixel::DebugMode::DEBUG_BSDF), 1);

    // Render image
    renderer->RenderImage(f, scene, *camera);

    // Create tone mapper
    pixel::ToneMapperInterface *t = new pixel::ClampToneMapper(1.f);
    // Process image and create it
    t->Process(std::string("test.ppm"), *f);

    return 0;
}