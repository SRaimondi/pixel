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
 * File:   material.h
 * Author: simon
 *
 * Created on October 31, 2016, 10:00 PM
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "pixel.h"
#include "sse_spectrum.h"
#include "interaction.h"

namespace pixel {

    enum MATERIAL_TYPE {
        MAT_SCATTERING = 0,
        MAT_EMITTING = 1
    };

    class MaterialInterface {
    public:
        // Constructor
        MaterialInterface(const MATERIAL_TYPE type)
                : type(type) {
        }

        // Destructor
        virtual ~MaterialInterface() {
        }

        // Creates the BSDF for a given SurfaceInteraction
        virtual BSDF *GetBSDF(const SurfaceInteraction &interaction) const = 0;

        // Evaluate the emission of the material at a given SurfaceInteraction in a given direction
        virtual SSESpectrum Emission(const SurfaceInteraction &, const SSEVector &) const {
            return SSESpectrum(0.f);
        }

        // Material type
        const MATERIAL_TYPE type;
    };

}

#endif /* MATERIAL_H */

