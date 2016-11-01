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
 * File:   intersectable.h
 * Author: simon
 *
 * Created on October 27, 2016, 12:07 AM
 */

#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "pixel.h"

namespace pixel {

    // Define shape interface
    class ShapeInterface {
    public:
        // Virtual destructor
        virtual ~ShapeInterface();

        // Check if a ray interacts with the shape and possibly fill a
        // surface_interaction class
        virtual bool Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const = 0;

        // Check if a ray interacts with the shape
        virtual bool IntersectP(const Ray &ray) const = 0;

        // Compute area of the shape
        virtual float Area() const = 0;

        // Sample a random point on the Shape
        virtual SurfaceInteraction Sample(float u1, float u2) const = 0;

        // Sample a random point on the Shape, from a given SurfaceInteraction
        virtual SurfaceInteraction Sample(const SurfaceInteraction &from, float u1, float u2) const;

        virtual float Pdf(const SurfaceInteraction &from) const;

        // Pdf from interaction and give wi
        virtual float Pdf(const SurfaceInteraction &from, const SSEVector &wi) const;

        // Returns the Shape BBOX
        virtual BBox ShapeBounding() const = 0;
    };
}

#endif /* INTERSECTABLE_H */

