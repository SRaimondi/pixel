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
 * File:   montecarlo.h
 * Author: simon
 *
 * Created on October 31, 2016, 9:13 PM
 */

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "pixel.h"
#include "sse_vector.h"

namespace pixel {

    // Cosine sample hemisphere
    inline SSEVector CosineSampleHemisphere(float u1, float u2) {
        // Sample phi
        float phi = u2 * TWO_PI;
        float x = std::cos(phi) * std::sqrt(u1);
        float z = std::sin(phi) * std::sqrt(u1);
        float y = std::sqrt(1.f - u1);

        return SSEVector(x, y, z, 0.f);
    }

    // Cosine hemisphere pdf
    inline float CosinePdfHemisphere(float cos_theta) {
        return (cos_theta * ONE_OVER_PI);
    }

    // Uniform sample hemisphere
    inline SSEVector UniformSampleHemisphere(float u1, float u2) {
        float y = u1;
        float r = std::sqrt(FMax(0.f, 1.f - y * y));
        float phi = TWO_PI * u2;

        return SSEVector(r * std::cos(phi), y, r * std::sin(phi), 0.f);
    }

    // Uniform hemisphere pdf
    inline float UniformPdfHemisphere() {
        return ONE_OVER_2_PI;
    }

    // Uniform sample sphere
    inline SSEVector UniformSampleSphere(float u1, float u2) {
        float y = 1.f - 2.f * u1;
        float r = std::sqrt(FMax(0.f, 1.f - y * y));
        float phi = TWO_PI * u2;

        return SSEVector(r * std::cos(phi), y, r * std::sin(phi), 0.f);
    }

    // Uniform sphere pdf
    inline float UniformPdfSphere() {
        return ONE_OVER_4_PI;
    }

}

#endif /* MONTECARLO_H */

