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


#include "whitted_integrator.h"
#include "interaction.h"
#include "sse_spectrum.h"
#include "scene.h"
#include "ray.h"


namespace pixel {

    WhittedIntegrator::WhittedIntegrator(uint32_t max_depth)
            : max_depth(max_depth) {
    }

    void WhittedIntegrator::Preprocess() const {

    }

    SSESpectrum WhittedIntegrator::IncomingRadiance(const Ray &ray, const Scene &scene) const {
        SSESpectrum L(0.f);
        // Find nearest intersection
        SurfaceInteraction interaction;
        if (!scene.Intersect(ray, &interaction)) {
            return L;
        }
        // Generate BSDF
        interaction.GenerateBSDF();
        // Compute wo
        SSEVector wo_world = Normalize(-ray.Direction());
        // Add emission
        L += interaction.EmittedRadiance(wo_world);
        // Compute direct illumination at found interaction
        L += DirectIllumination(interaction, wo_world, scene);

        if (ray.RayDepth() < max_depth) {
            L += SpecularReflection(interaction, wo_world, this, scene, ray.RayDepth() + 1);
            L += SpecularRefraction(interaction, wo_world, this, scene, ray.RayDepth() + 1);
        }

        return L;
    }

}
