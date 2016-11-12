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
 * File:   interaction.h
 * Author: simon
 *
 * Created on October 26, 2016, 11:56 PM
 */

#ifndef INTERACTION_H
#define INTERACTION_H

#include "pixel.h"
#include "sse_vector.h"
#include "shape.h"
#include "primitive.h"
#include "scattering.h"

namespace pixel {

    // Define surface_interaction class
    class SurfaceInteraction {
    public:
        // Constructor
        SurfaceInteraction();

        SurfaceInteraction(const SSEVector &hit, const SSEVector &n,
                           const SSEVector &s, const SSEVector &t, float u, float v,
                           const PrimitiveInterface *const prim_ptr,
                           const MaterialInterface *const mat_ptr);

        // Compute emission at interaction itypen given direction
        SSESpectrum EmittedRadiance(const SSEVector &w) const;

        // Spawn ray in given direction
        Ray SpawnRay(const SSEVector &dir, uint32_t depth = 0) const;

        // Generate the BSDF
        void GenerateBSDF();

        // Hit point
        SSEVector hit_point;
        // Geometric normal at hit point
        SSEVector normal;
        // Local tangent vectors
        SSEVector s, t;
        // UV coordinates
        float u, v;
        // Primitive hit
        const PrimitiveInterface *prim_ptr;
        // Primitive material
        const MaterialInterface *mat_ptr;
        // BSDF
        std::unique_ptr<const BSDF> bsdf;
    };

    // Transform surface_interaction for a given matrix
    void TransformSurfaceInteraction(SurfaceInteraction *const interaction,
                                     const SSEMatrix &mat);

}

#endif /* INTERACTION_H */

