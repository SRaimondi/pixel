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

#include <bbox.h>
#include "instance.h"
#include "ray.h"
#include "shape.h"
#include "interaction.h"

namespace pixel {

    Instance::Instance(const ShapeInterface *const s,
                       const MaterialInterface *const m,
                       const SSEMatrix &l2w, const SSEMatrix &w2l)
            : shape(s), material(m), local_to_world(l2w), world_to_local(w2l) {
    }

    Instance::Instance(const ShapeInterface *const s,
                       const MaterialInterface *const m,
                       const SSEMatrix &l2w)
            : shape(s), material(m), local_to_world(l2w), world_to_local(Inverse(local_to_world)) {
    }

    bool Instance::Intersect(const Ray &ray, SurfaceInteraction *const interaction) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);
        float t_hit;
        if (shape->Intersect(local_ray, &t_hit, interaction)) {
            // Transform back SurfaceInteraction
            TransformSurfaceInteraction(interaction, local_to_world);
            // Update ray maximum value
            ray.SetNewMaximum(t_hit);
            interaction->prim_ptr = this;
            interaction->mat_ptr = material;

            return true;
        }

        return false;
    }

    bool Instance::IntersectP(const Ray &ray) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);

        return shape->IntersectP(local_ray);
    }

    BBox Instance::PrimitiveBounding() const {
        // Compute transformed shape BBOX
        BBox bbox = shape->ShapeBounding();
        // Compute all eight BBox vertices
        SSEVector vertices[8];
        vertices[0] = bbox.Min();
        vertices[1] = SSEVector(bbox.Max().x, bbox.Min().y, bbox.Min().z, 1.f);
        vertices[2] = SSEVector(bbox.Max().x, bbox.Min().y, bbox.Max().z, 1.f);
        vertices[3] = SSEVector(bbox.Min().x, bbox.Min().y, bbox.Max().z, 1.f);

        vertices[4] = SSEVector(bbox.Min().x, bbox.Max().y, bbox.Min().z, 1.f);
        vertices[5] = SSEVector(bbox.Max().x, bbox.Max().y, bbox.Min().z, 1.f);
        vertices[6] = bbox.Max();
        vertices[7] = SSEVector(bbox.Min().x, bbox.Max().y, bbox.Max().z, 1.f);

        // Reset BBox and compute the primitive one
        bbox = BBox();
        for (uint32_t i = 0; i < 8; i++) {
            bbox = BBoxUnion(bbox, vertices[i]);
        }

        return bbox;
    }


}