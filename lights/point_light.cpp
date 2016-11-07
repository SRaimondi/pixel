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

#include "point_light.h"

namespace pixel {

    PointLight::PointLight(const SSEVector &p, const SSESpectrum &i)
    : position(p), intensity(i) {}

    bool PointLight::IsDeltaLight() const {
        return true;
    }

    SSESpectrum PointLight::Sample_Li(const SurfaceInteraction &from, float , float ,
                                      SSEVector *const wi, float *const pdf, OcclusionTester *const occ) const {
        *wi = Normalize(position - from.hit_point);
        *pdf = 1.f;
        *occ = OcclusionTester(from, position);

        return (intensity / SSESqrdLength(position - from.hit_point));
    }

    float PointLight::Pdf_Li(const SurfaceInteraction &, const SSEVector &) const {
        return 0.f;
    }

}