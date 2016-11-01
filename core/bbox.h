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
 * File:   bbox.h
 * Author: simon
 *
 * Created on October 31, 2016, 6:00 PM
 */

#ifndef BBOX_H
#define BBOX_H

#include "pixel.h"
#include "sse_vector.h"

namespace pixel {

    // Define Bbox class

    class BBox {
    public:
        // Constructor
        BBox();

        BBox(const SSEVector &p);

        BBox(const SSEVector &p1, const SSEVector &p2);

        // Access minimum and maximum

        inline const SSEVector &Min() const {
            return min;
        }

        inline const SSEVector &Max() const {
            return max;
        }

        // Access bounding points by index
        inline const SSEVector &operator[](uint32_t i) const {
            return (i == 0) ? min : max;
        }

        inline SSEVector &operator[](uint32_t i) {
            return (i == 0) ? min : max;
        }

    private:
        // Maximum and minimum
        SSEVector min, max;
    };

    // Compute the union between two BBox
    BBox BBoxUnion(const BBox &b1, const BBox &b2);

    // Compute the union between a BBox and a point
    BBox BBoxUnion(const BBox &b, const SSEVector &p);

}

#endif /* BBOX_H */

