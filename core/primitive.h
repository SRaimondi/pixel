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
 * File:   primitive.h
 * Author: simon
 *
 * Created on October 31, 2016, 4:55 PM
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "pixel.h"

namespace pixel {

    // Define primitive interface
    class PrimitiveInterface {
    public:
        // Destructor
        virtual ~PrimitiveInterface() {
        }

        // Check if a ray interacts with the primitive and possibly fill a
        // surface_interaction class
        virtual bool Intersect(const Ray &ray, SurfaceInteraction *const interaction) const = 0;

        // Check if a ray interacts with the primitive
        virtual bool IntersectP(const Ray &ray) const = 0;

        // Create primitive BBOX
        virtual BBox PrimitiveBounding() const = 0;

    };

}

#endif /* PRIMITIVE_H */

