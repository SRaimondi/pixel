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

#include "bbox.h"

namespace pixel {

    BBox::BBox() {
        bounds[0] = SSEVector(INFINITY, INFINITY, INFINITY, 1.f);
        bounds[1] = SSEVector(-INFINITY, -INFINITY, -INFINITY, 1.f);
    }

    BBox::BBox(const SSEVector &p) {
        bounds[0] = p;
        bounds[1] = p;
    }

    BBox::BBox(const SSEVector &p1, const SSEVector &p2) {
        bounds[0] = SSEVector(FMin(p1.x, p2.x), FMin(p1.y, p2.y), FMin(p1.z, p2.z), 1.f);
        bounds[1] = SSEVector(FMax(p1.x, p2.x), FMax(p1.y, p2.y), FMax(p1.z, p2.z), 1.f);
    }

    BBox BBoxUnion(const BBox &b1, const BBox &b2) {
        return BBox(
                SSEVector(FMin(b1.Min().x, b2.Min().x), FMin(b1.Min().y, b2.Min().y), FMin(b1.Min().z, b2.Min().z),
                          1.f),
                SSEVector(FMin(b1.Max().x, b2.Max().x), FMin(b1.Max().y, b2.Max().y), FMin(b1.Max().z, b2.Max().z),
                          1.f));
    }

    BBox BBoxUnion(const BBox &b, const SSEVector &p) {
        return BBox(
                SSEVector(FMin(b.Min().x, p.x), FMin(b.Min().y, p.y), FMin(b.Min().z, p.z), 1.f),
                SSEVector(FMin(b.Max().x, p.x), FMin(b.Max().y, p.y), FMin(b.Max().z, p.z), 1.f));
    }

}