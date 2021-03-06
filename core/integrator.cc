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

#include "integrator.h"
#include "sse_spectrum.h"
#include "interaction.h"
#include "scene.h"
#include "light.h"
#include "scattering.h"
#include "ray.h"

// DEBUG
#include <random>

namespace pixel {

    // TODO: FIX ME
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.f, 1.f);

    SSESpectrum
    DirectIllumination(const SurfaceInteraction &interaction, const SSEVector &wo_world, const Scene &scene) {
        SSESpectrum Ld(0.f);

        // Type of BRDF to check for direct illumination
        BRDF_TYPE brdf_types = BRDF_TYPE(ALL_BRDF & ~BRDF_SPECULAR);
        // Sample lights
        SSEVector wi;
        float pdf_Li;
        OcclusionTester occ_tester;
        for (auto light : scene.GetLights()) {
            // Sample incoming radiance
            SSESpectrum Li = light->Sample_Li(interaction, distribution(generator), distribution(generator), &wi,
                                              &pdf_Li, &occ_tester);
            if (!IsBlack(Li) && pdf_Li != 0.f) {
                if (occ_tester.Unoccluded(scene)) {
                    // Evaluate BRDF
                    SSESpectrum f = interaction.bsdf->f(wo_world, wi, brdf_types);
                    if (!IsBlack(f)) {
                        Ld += f * Li * AbsDotProductSSE(wi, interaction.normal) / pdf_Li;
                    }
                }
            }
        }

        return Ld;
    }

    SSESpectrum SpecularReflection(const SurfaceInteraction &interaction, const SSEVector &wo_world,
                                   const SurfaceIntegratorInterface *const integrator, const Scene &scene,
                                   uint32_t depth) {
        SSESpectrum Ls(0.f);
        // Type of BRDF to check for direct illumination
        BRDF_TYPE brdf_types = BRDF_TYPE(BRDF_REFLECTION | BRDF_SPECULAR);
        // Sample specular BRDF
        SSEVector world_wi;
        float pdf;
        SSESpectrum f = interaction.bsdf->Sample_f(wo_world, &world_wi, &pdf, distribution(generator),
                                                   distribution(generator), brdf_types);
        if (pdf > 0.f && !IsBlack(f)) {
            // Create specular ray
            Ray specular_ray = interaction.SpawnRay(world_wi, depth);
            Ls = f * integrator->IncomingRadiance(specular_ray, scene) / pdf;
        }

        return Ls;
    }

    SSESpectrum SpecularRefraction(const SurfaceInteraction &interaction, const SSEVector &wo_world,
                                   const SurfaceIntegratorInterface *const integrator, const Scene &scene,
                                   uint32_t depth) {
        SSESpectrum Ls(0.f);
        // Type of BRDF to check for direct illumination
        BRDF_TYPE brdf_types = BRDF_TYPE(BRDF_TRANSMISSION | BRDF_SPECULAR);
        // Sample specular BRDF
        SSEVector world_wi;
        float pdf;
        SSESpectrum f = interaction.bsdf->Sample_f(wo_world, &world_wi, &pdf, distribution(generator),
                                                   distribution(generator), brdf_types);
        if (pdf > 0.f && !IsBlack(f)) {
            // Create specular ray
            Ray specular_ray = interaction.SpawnRay(world_wi, depth);
            Ls = f * integrator->IncomingRadiance(specular_ray, scene) / pdf;
        }

        return Ls;
    }

}