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
 * File:   prim_list.h
 * Author: simon
 *
 * Created on November 1, 2016, 12:27 AM
 */

#ifndef PRIM_LIST_H
#define PRIM_LIST_H

#include "pixel.h"
#include "primitive.h"

namespace pixel {

    // Define Primitive list class
    class PrimitiveList : public PrimitiveInterface {
    public:
        // Constructor
        PrimitiveList();

        // Add primitive
        void AddPrimitive(const PrimitiveInterface *p);

        bool Intersect(const Ray &ray, SurfaceInteraction *const interaction) const override;

        bool IntersectP(const Ray &ray) const override;

    private:
        // List of primitive
        std::vector<const PrimitiveInterface *> primitives;
    };

}

#endif /* PRIM_LIST_H */

