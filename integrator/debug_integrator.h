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
 * File:   debug_integrator.h
 * Author: simon
 *
 * Created on October 28, 2016, 12:42 AM
 */

#ifndef DEBUG_INTEGRATOR_H
#define DEBUG_INTEGRATOR_H

#include "pixel.h"
#include "integrator.h"

namespace pixel {

    // Debug integrator modes
    enum class DebugMode {
        DEBUB_HIT,
        DEBUG_NORMAL,
        DEBUG_WO,
        DEBUG_BSDF
    };

    // Define debug integrator class
    class DebugIntegrator : public SurfaceIntegratorInterface {
    public:
        // Constructor
        DebugIntegrator(const DebugMode &mode);

        void Preprocess() const override;

        // Compute incoming radiance from a given ray
        SSESpectrum IncomingRadiance(const Ray &ray, const Scene &scene) const override;

    private:
        DebugMode mode;
    };

}

#endif /* DEBUG_INTEGRATOR_H */

