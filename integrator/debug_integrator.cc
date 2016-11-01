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

#include "debug_integrator.h"
#include "sse_spectrum.h"
#include "scene.h"
#include "interaction.h"
#include "scattering.h"
#include "ray.h"

namespace pixel {

    DebugIntegrator::DebugIntegrator(const DebugMode &mode)
            : SurfaceIntegratorInterface(), mode(mode) {
    }

    void DebugIntegrator::Preprocess() const {

    }

    SSESpectrum DebugIntegrator::IncomingRadiance(const Ray &ray, const Scene &scene) const {
        SSESpectrum L;

        switch (mode) {
            case DebugMode::DEBUB_HIT: {
                SurfaceInteraction interaction;
                if (scene.Intersect(ray, &interaction)) {
                    L = SSESpectrum(1.f, 0.f, 0.f);
                }
                break;
            }

            case DebugMode::DEBUG_NORMAL: {
                SurfaceInteraction interaction;
                if (scene.Intersect(ray, &interaction)) {
                    L = SSESpectrum(std::abs(interaction.normal.x),
                                    std::abs(interaction.normal.y),
                                    std::abs(interaction.normal.z));
                }
                break;
            }

            case DebugMode::DEBUG_WO: {
                SurfaceInteraction interaction;
                if (scene.Intersect(ray, &interaction)) {
                    L = SSESpectrum(std::abs(-ray.Direction().x),
                                    std::abs(-ray.Direction().y),
                                    std::abs(-ray.Direction().z));
                }
                break;
            }

            case DebugMode::DEBUG_BSDF: {
                SurfaceInteraction interaction;
                if (scene.Intersect(ray, &interaction)) {
                    // Get BSDF
                    BSDF *bsdf = interaction.GetBSDF();
                    // Fixed vertical light direction and power
                    SSESpectrum Li(5.f);
                    SSEVector wi(0.f, 1.f, 0.f, 0.f);
                    // Evaluate BSDF
                    SSESpectrum f = bsdf->f(-Normalize(ray.Direction()), wi);
                    L = f * Li * AbsDotProductSSE(wi, interaction.normal);
                    delete bsdf;
                }
                break;
            }
        }

        return L;
    }

}