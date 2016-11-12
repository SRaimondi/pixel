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

#include "path_tracer_integrator.h"
#include "interaction.h"
#include "sse_spectrum.h"
#include "scene.h"
#include "ray.h"
#include "scattering.h"

#include <random>

namespace pixel {

    // TODO: FIX ME
    std::default_random_engine generator_path;
    std::uniform_real_distribution<float> distribution_path(0.f, 1.f);

    PathTracerIntegrator::PathTracerIntegrator(uint32_t max_depth)
            : max_depth(max_depth) {
    }

    void PathTracerIntegrator::Preprocess() const {

    }

    SSESpectrum PathTracerIntegrator::IncomingRadiance(const Ray &ray, const Scene &scene) const {
        SSESpectrum L(0.f);
        SSESpectrum alpha(1.f);
        // Current ray
        Ray current_ray = ray;
        // Current outgoing direction
        SSEVector wo_world;
        // Sampling BSDF
        SSEVector wi_world;
        float pdf;
        BRDF_TYPE brdf_type;
        bool specular_hit = false;
        SurfaceInteraction interaction;
        for (uint32_t bounce = 0; bounce < max_depth; bounce++) {
            if (!scene.Intersect(current_ray, &interaction)) { break; }
            interaction.GenerateBSDF();
            // Compute wo
            wo_world = Normalize(-current_ray.Direction());
            // Check for emission
            if (bounce == 0 || specular_hit) {
                L += alpha * interaction.EmittedRadiance(wo_world);
            }
            // Compute direct illumination
            L += alpha * DirectIllumination(interaction, wo_world, scene);
            // Sample the BSDF
            interaction.GenerateBSDF();
            SSESpectrum f = interaction.bsdf->Sample_f(wo_world, &wi_world, &pdf,
                                                       distribution_path(generator_path),
                                                       distribution_path(generator_path),
                                                       ALL_BRDF, &brdf_type);
            if (IsBlack(f) || pdf == 0.f) {
                break;
            }
            specular_hit = (brdf_type & BRDF_SPECULAR) != 0;
            // Compute cosine term
            float cos_wi = specular_hit ? 1.f : AbsDotProduct(wi_world, interaction.normal);
            // Update alpha
            alpha *= f * (cos_wi / pdf);
            // Update ray
            current_ray = interaction.SpawnRay(wi_world);
        }

        return L;
    }

}
