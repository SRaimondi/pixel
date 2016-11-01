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

#include "shape_list.h"
#include "interaction.h"
#include "bbox.h"

namespace pixel {

    ShapeList::ShapeList() {
    }

    ShapeList::ShapeList(const std::vector<const ShapeInterface *> s)
            : shapes(s) {
    }

    void ShapeList::AddShape(const ShapeInterface *s) {
        shapes.push_back(s);
    }

    bool ShapeList::Intersect(const Ray &ray, SurfaceInteraction *const interaction) const {
        SurfaceInteraction temp_interaction;
        float nearest_t = INFINITY;
        bool hit = false;
        for (auto s : shapes) {
            if (s->Intersect(ray, &temp_interaction) && temp_interaction.t_param < nearest_t) {
                *interaction = temp_interaction;
                nearest_t = temp_interaction.t_param;
                hit = true;
            }
        }

        return hit;
    }

    bool ShapeList::IntersectP(const Ray &ray) const {
        for (auto s : shapes) {
            if (s->IntersectP(ray)) {
                return true;
            }
        }

        return false;
    }

    BBox ShapeList::ShapeBounding() const {
        BBox boundig;
        for (auto s : shapes) {
            boundig = BBoxUnion(boundig, s->ShapeBoundig());
        }

        return boundig;
    }


}