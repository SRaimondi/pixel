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
 * File:   instance.h
 * Author: simon
 *
 * Created on November 6, 2016, 11:58 PM
 */

#ifndef PIXEL_AREA_LIGHT_H
#define PIXEL_AREA_LIGHT_H

#include "pixel.h"
#include "light.h"
#include "primitive.h"
#include "sse_matrix.h"

namespace pixel {

    // Define AreaLight class

    class AreaLight : public LightInterface, public PrimitiveInterface {
    public:
        // Constructor
        AreaLight(const ShapeInterface *const s,
                  const MaterialInterface *const m);

        SSESpectrum Sample_Li(const SurfaceInteraction &from, float u1, float u2,
                              SSEVector *const wi, float *const pdf, OcclusionTester *const occ) const override;

        float Pdf_Li(const SurfaceInteraction &from, const SSEVector &wi) const override;

        bool Intersect(const Ray &ray, SurfaceInteraction *const interaction) const override;

        bool IntersectP(const Ray &ray) const override;

        BBox PrimitiveBounding() const override;

    private:
        // Transformed Shape representing the light
        const ShapeInterface *shape;
        // Light material
        const MaterialInterface *material;
    };

}

#endif //PIXEL_AREA_LIGHT_H
