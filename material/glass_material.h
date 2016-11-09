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

/*
 * File:   glass_material.h
 * Author: simon
 *
 * Created on November 8, 2016, 10:50 PM
 */

#ifndef PIXEL_GLASS_MATERIAL_H
#define PIXEL_GLASS_MATERIAL_H

#include "pixel.h"
#include "material.h"

namespace pixel {

    class GlassMaterial : public MaterialInterface {
    public:
        GlassMaterial(const SSESpectrum &R, const SSESpectrum &T, float i);

        BSDF *GetBSDF(const SurfaceInteraction &interaction) const override;

    private:
        // Reflection
        const SSESpectrum R;
        // Transmission
        const SSESpectrum T;
        // Refraction index
        const float r_index;
    };

}

#endif //PIXEL_GLASS_MATERIAL_H
