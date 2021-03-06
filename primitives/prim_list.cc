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

#include "prim_list.h"
#include "bbox.h"

namespace pixel {

    PrimitiveList::PrimitiveList()
            : primitives() {
    }

    void PrimitiveList::AddPrimitive(const PrimitiveInterface *const p) {
        primitives.push_back(p);
        // primitives.push_back(std::make_shared<const PrimitiveInterface>(p));
    }

    bool PrimitiveList::Intersect(const Ray &ray, SurfaceInteraction *const interaction) const {
        bool hit = false;
        for (auto prim : primitives) {
            if (prim->Intersect(ray, interaction)) { hit = true; }
        }

        return hit;
    }

    bool PrimitiveList::IntersectP(const Ray &ray) const {
        for (auto prim : primitives) {
            if (prim->IntersectP(ray)) {
                return true;
            }
        }

        return false;
    }

    BBox PrimitiveList::PrimitiveBounding() const {
        BBox bbox;
        for (auto prim : primitives) {
            bbox = BBoxUnion(bbox, prim->PrimitiveBounding());
        }

        return bbox;
    }

}
