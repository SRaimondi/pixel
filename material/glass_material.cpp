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

#include "glass_material.h"
#include "scattering.h"

namespace pixel {

    GlassMaterial::GlassMaterial(const SSESpectrum &R, const SSESpectrum &T, float i)
            : MaterialInterface(MAT_SCATTERING), R(R), T(T), r_index(i) {
    }

    std::unique_ptr<BSDF> GlassMaterial::GetBSDF(const SurfaceInteraction &interaction) const {
        // Allocate BSDF
        // BSDF *bsdf = new BSDF(interaction);
        auto bsdf = std::make_unique<BSDF>(interaction);

        // Add simple Fresnel specular BRDF
        if (!IsBlack(R) && !IsBlack(T)) {
            bsdf->AddBRDF(new SpecularReflection(R, new FresnelDielectric(1.f, r_index)));
            bsdf->AddBRDF(new SpecularTransmission(T, 1.f, r_index));
        }

        return bsdf;
    }

}