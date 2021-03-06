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

    GlassMaterial::GlassMaterial(const std::shared_ptr<const TextureInterface<SSESpectrum>> &R,
                                 const std::shared_ptr<const TextureInterface<SSESpectrum>> &T,
                                 const std::shared_ptr<const TextureInterface<float>> &i)
            : MaterialInterface(MAT_SCATTERING), R(R), T(T), r_index(i) {
    }

    std::unique_ptr<BSDF> GlassMaterial::GetBSDF(const SurfaceInteraction &interaction) const {
        // Allocate BSDF
        auto bsdf = std::make_unique<BSDF>(interaction);
        // Evaluate textures
        const SSESpectrum ref = R->Evaluate(interaction);
        const SSESpectrum trans = T->Evaluate(interaction);
        const float r_i = r_index->Evaluate(interaction);
        // Add simple Fresnel specular BRDF
        if (!IsBlack(ref) && !IsBlack(trans)) {
            bsdf->AddBRDF(new SpecularReflection(ref, new FresnelDielectric(1.f, r_i)));
            bsdf->AddBRDF(new SpecularTransmission(trans, 1.f, r_i));
        }

        return bsdf;
    }

}
