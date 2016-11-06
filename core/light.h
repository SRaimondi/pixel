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
 * File:   light.h
 * Author: simon
 *
 * Created on November 06, 2016, 9:55 PM
 */

#ifndef PIXEL_LIGHT_H
#define PIXEL_LIGHT_H

#include "pixel.h"
#include "sse_matrix.h"
#include "interaction.h"

namespace pixel {

    // Define light interface
    class LightInterface {
    public:
        // Virtual destructor
        virtual ~LightInterface();

        // Returns true if light is delta light
        virtual bool IsDeltaLight() const;

        // Sample incoming light at a given SurfaceInteraction
        virtual SSESpectrum Sample_Li(const SurfaceInteraction &from, float u1, float u2,
                                      SSEVector *const wi, float *const pdf, OcclusionTester *const occ) const = 0;

        // Compute directional PDF to sample the given light direction from a given SurfaceInteraction
        virtual float Pdf_Li(const SurfaceInteraction &from, const SSEVector &wi) const = 0;

    };

    // Define occlusion tester class
    class OcclusionTester {
    public:
        // Constructor
        OcclusionTester();

        OcclusionTester(const SurfaceInteraction &si1, const SurfaceInteraction &si2);

        // Check if the ray between the two interaction is occluded or not
        bool Unoccluded(const Scene &scene) const;

    private:
        SurfaceInteraction si1, si2;
    };

}

#endif //PIXEL_LIGHT_H
