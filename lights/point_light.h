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
 * File:   point_light.h
 * Author: simon
 *
 * Created on November 6, 2016, 10:26 PM
 */

#ifndef PIXEL_POINT_LIGHT_H
#define PIXEL_POINT_LIGHT_H

#include "pixel.h"
#include "light.h"
#include "sse_spectrum.h"

namespace pixel {

    class PointLight : public LightInterface {
    public:
        // Constructor
        PointLight(const SSEVector &p, const SSESpectrum &i);

        bool IsDeltaLight() const override;

        SSESpectrum Sample_Li(const SurfaceInteraction &from, float u1, float u2,
                              SSEVector *const wi, float *const pdf, OcclusionTester *const occ) const override;

        float Pdf_Li(const SurfaceInteraction &from, const SSEVector &wi) const override;

    private:
        // Light position
        const SSEVector position;
        // Point light intensity
        const SSESpectrum intensity;
    };

}

#endif //PIXEL_POINT_LIGHT_H
