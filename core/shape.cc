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

#include "shape.h"
#include "interaction.h"
#include "ray.h"
#include "bbox.h"

namespace pixel {

    ShapeInterface::ShapeInterface(const SSEMatrix &l2w, const SSEMatrix &w2l)
    : local_to_world(l2w), world_to_local(w2l) {
    }

    ShapeInterface::ShapeInterface(const SSEMatrix &l2w)
    : local_to_world(l2w), world_to_local(Inverse(local_to_world)) {
    }

    ShapeInterface::~ShapeInterface() {
    }

    SurfaceInteraction ShapeInterface::Sample(const SurfaceInteraction &, float u1, float u2) const {
        return Sample(u1, u2);
    }

    float ShapeInterface::Pdf(const SurfaceInteraction &) const {
        return (1.f / Area());
    }

    float ShapeInterface::Pdf(const SurfaceInteraction &from, const SSEVector &wi) const {
        // Create ray in given direction and test for intersection
        Ray ray = from.SpawnRay(wi);
        SurfaceInteraction interaction_light;
        float t_hit;
        // Compute intersection
        if (!Intersect(ray, &t_hit, &interaction_light)) { return 0.f; }

        // Convert area pdf to solid angle
        float pdf = SqrdLength(interaction_light.hit_point - from.hit_point) /
                    (AbsDotProduct(interaction_light.normal, -wi) * Area());
        if (std::isinf(pdf)) { pdf = 0.f; }

        return pdf;
    }

    BBox ShapeInterface::WorldBounding() const {
        // Compute transformed shape BBOX
        BBox bbox = ShapeBounding();
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