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

#include "matte_material.h"
#include "texture.h"

namespace pixel {

    MatteMaterial::MatteMaterial(const std::shared_ptr<const TextureInterface<SSESpectrum>> &Kd,
                                 const std::shared_ptr<const TextureInterface<float>> &s)
            : MaterialInterface(MAT_SCATTERING), Kd(Kd), sigma(s) {
    }

    std::unique_ptr<BSDF> MatteMaterial::GetBSDF(const SurfaceInteraction &interaction) const {
        // Allocate BSDF
        auto bsdf = std::make_unique<BSDF>(interaction);
        // Evaluate textures
        const SSESpectrum rho = Kd->Evaluate(interaction);
        const float sig = sigma->Evaluate(interaction);
        if (!IsBlack(rho)) {
            if (sig == 0) {
                bsdf->AddBRDF(new LambertianReflection(rho));
            } else {
                bsdf->AddBRDF(new OrenNayar(rho, sig));
            }
        }

        return bsdf;
    }

}
