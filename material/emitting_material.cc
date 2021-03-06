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

#include "emitting_material.h"
#include "texture.h"

namespace pixel {

    EmittingMaterial::EmittingMaterial(const std::shared_ptr<const TextureInterface<SSESpectrum>> &e)
            : MaterialInterface(MAT_EMITTING), emission(e) {
    }

    std::unique_ptr<BSDF> EmittingMaterial::GetBSDF(const SurfaceInteraction &interaction) const {
        // Return empty BSDF
        auto bsdf = std::make_unique<BSDF>(interaction);

        return bsdf;
    }

    SSESpectrum EmittingMaterial::Emission(const SurfaceInteraction &interaction, const SSEVector &w) const {
        float dot = DotProduct(w, interaction.normal);
        const SSESpectrum e = emission->Evaluate(interaction);
        if (dot > 0.f && !IsBlack(e)) {
            return SSESpectrum(dot * e);
        } else {
            return SSESpectrum(0.f);
        }
    }

}
