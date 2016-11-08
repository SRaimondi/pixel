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

#include "instance.h"
#include "ray.h"
#include "shape.h"
#include "interaction.h"
#include "bbox.h"

namespace pixel {

    Instance::Instance(const ShapeInterface *const s,
                       const MaterialInterface *const m)
            : shape(s), material(m) {
    }

    bool Instance::Intersect(const Ray &ray, SurfaceInteraction *const interaction) const {
        float t_hit;
        if (shape->Intersect(ray, &t_hit, interaction)) {
            // Update ray maximum value
            ray.SetNewMaximum(t_hit);
            interaction->prim_ptr = this;
            interaction->mat_ptr = material;

            return true;
        }

        return false;
    }

    bool Instance::IntersectP(const Ray &ray) const {
        return shape->IntersectP(ray);
    }

    BBox Instance::PrimitiveBounding() const {
        return shape->WorldBounding();
    }


}