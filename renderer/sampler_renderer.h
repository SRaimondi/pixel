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
 * File:   sampler_renderer.h
 * Author: simon
 *
 * Created on October 28, 2016, 12:22 AM
 */

#ifndef SAMPLER_RENDERER_H
#define SAMPLER_RENDERER_H

#include "pixel.h"
#include "renderer.h"

namespace pixel {

    // Define SamplerRenderer class which uses multiple sample per pixel to generate rays
    class SamplerRenderer : public RendererInterface {
    public:
        // Constructor
        SamplerRenderer(SurfaceIntegratorInterface *const integrator, uint32_t aa_samples);

        // Render scene given a film, a scene and a camera
        void RenderImage(Film *const film, const Scene &scene, const CameraInterface &camera) const override;

    private:
        // Number of sampler to trace per pixel
        const uint32_t aa_samples;
    };

}

#endif /* SAMPLER_RENDERER_H */

