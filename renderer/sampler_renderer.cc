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

#include "sampler_renderer.h"
#include "film.h"
#include "camera.h"
#include "ray.h"

namespace pixel {

    SamplerRenderer::SamplerRenderer(SurfaceIntegratorInterface *const integrator, uint32_t aa_samples)
            : RendererInterface(integrator), aa_samples(aa_samples) {
    }

    void SamplerRenderer::RenderImage(Film *const film, const Scene &scene, const CameraInterface &camera) const {
        // Loop over all image pixels
        for (uint32_t i = 0; i < film->GetWidth(); i++) {
            for (uint32_t j = 0; j < film->GetHeight(); j++) {
                // Loop over all samples
                for (uint32_t s = 0; s < aa_samples; s++) {
                    // Request ray from camera
                    Ray ray = camera.GenerateRay(i, j, 0.5f, 0.5f); // TODO hardcoded mid of the pixel
                    // Integrate ray
                    SSESpectrum Li = integrator->IncomingRadiance(ray, scene);
                    // Add sampler
                    film->AddSample(Li, i + 0.5f, j + 0.5f);
                }
            }
        }
    }

}