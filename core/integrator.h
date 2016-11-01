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
 * File:   integrator.h
 * Author: simon
 *
 * Created on October 27, 2016, 12:47 AM
 */

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "pixel.h"

namespace pixel {

    // Define base integrator class
    class IntegratorInterface {
    public:
        // Preprocess
        virtual void Preprocess() const = 0;
    };

    // Define surface integrator base class
    class SurfaceIntegratorInterface : public IntegratorInterface {
    public:
        // Compute incoming radiance from a given ray
        virtual SSESpectrum IncomingRadiance(const Ray &ray, const Scene &scene) const = 0;
    };

    // Estimate direct illumination at given SurfaceInteraction
    SSESpectrum DirectIllumination(const SurfaceInteraction &interaction, const Scene &scene);

}

#endif /* INTEGRATOR_H */

