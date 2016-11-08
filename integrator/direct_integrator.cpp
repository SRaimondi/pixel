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

#include "interaction.h"
#include "direct_integrator.h"
#include "sse_spectrum.h"
#include "scene.h"
#include "ray.h"

namespace pixel {

    DirectIntegrator::DirectIntegrator() {
    }

    void DirectIntegrator::Preprocess() const {

    }

    SSESpectrum DirectIntegrator::IncomingRadiance(const Ray &ray, const Scene &scene) const {
        SSESpectrum L(0.f);
        // Find nearest intersection
        SurfaceInteraction interaction;
        if (!scene.Intersect(ray, &interaction)) {
            return L;
        }
        // Compute wo
        SSEVector wo_world = Normalize(-ray.Direction());
        // Add emission
        L += interaction.EmittedRadiance(wo_world);
        // Compute direct illumination at found interaction
        L += DirectIllumination(interaction, wo_world, scene);

        return L;
    }

}