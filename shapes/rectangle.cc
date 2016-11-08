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

#include "rectangle.h"
#include "sse_vector.h"
#include "interaction.h"
#include "ray.h"
#include "bbox.h"

namespace pixel {

    Rectangle::Rectangle(const SSEMatrix &l2w, float x_w, float z_w)
            : ShapeInterface(l2w), half_x_width(x_w / 2.f), half_z_width(z_w / 2.f) {
    }

    bool Rectangle::Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);
        // Check if ray direction is parallel to plane
        if (std::abs(local_ray.Direction().y) > EPS) {
            // Compute intersection parameter
            float t = -local_ray.Origin().y / local_ray.Direction().y;
            if (t > local_ray.RayMinimum() && t < local_ray.RayMaximum()) {
                // Compute hit point
                SSEVector hit_p = local_ray(t);
                if (hit_p.x >= -half_x_width && hit_p.x <= half_x_width &&
                    hit_p.z >= -half_z_width && hit_p.z <= half_z_width) {
                    // Fill interaction data
                    *t_hit = t;
                    interaction->hit_point = hit_p;
                    interaction->normal = SSEVector(0.f, 1.f, 0.f, 0.f);
                    interaction->s = SSEVector(1.f, 0.f, 0.f, 0.f);
                    interaction->t = SSEVector(0.f, 0.f, 1.f, 0.f);
                    interaction->u = (hit_p.x + half_x_width) / (2.f * half_x_width);
                    interaction->v = (hit_p.z + half_z_width) / (2.f * half_z_width);

                    // Transform interaction back to world space
                    TransformSurfaceInteraction(interaction, local_to_world);

                    return true;
                }
            }
        }

        return false;
    }

    bool Rectangle::IntersectP(const Ray &ray) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);
        // Check if ray direction is parallel to plane
        if (std::abs(local_ray.Direction().y) > EPS) {
            // Compute intersection parameter
            float t = -local_ray.Origin().y / local_ray.Direction().y;
            if (t > local_ray.RayMinimum() && t < local_ray.RayMaximum()) {
                // Compute hit point
                SSEVector hit_p = ray(t);
                if (hit_p.x >= -half_x_width && hit_p.x <= half_x_width &&
                    hit_p.z >= -half_z_width && hit_p.z <= half_z_width) {
                    return true;
                }
            }
        }

        return false;
    }

    float Rectangle::Area() const {
        return (4.f * half_x_width * half_z_width);
    }

    SurfaceInteraction Rectangle::Sample(float u1, float u2) const {
        SurfaceInteraction interaction;
        float x_width = 2.f * half_x_width;
        float z_width = 2.f * half_z_width;
        // Just set hit_point and normal
        interaction.hit_point = local_to_world * SSEVector(-half_x_width + x_width * u1, 0.f, -half_z_width + z_width * u2, 1.f);
        interaction.normal = Normalize(Transpose(world_to_local) * SSEVector(0.f, 1.f, 0.f, 0.f));

        return interaction;
    }


    BBox Rectangle::ShapeBounding() const {
        return BBox(
                SSEVector(-half_x_width, -EPS, -half_z_width, 1.f),
                SSEVector(half_x_width, EPS, half_z_width, 1.f));
    }

}