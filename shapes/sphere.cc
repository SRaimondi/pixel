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

#include "sphere.h"
#include "ray.h"
#include "interaction.h"
#include "bbox.h"

namespace pixel {

    Sphere::Sphere(const SSEVector &c, float r)
            : center(c), radius(r) {
    }

    bool Sphere::Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const {
        SSEVector center_ray_origin = ray.Origin() - center;
        // Compute terms for quadratic form
        float a = DotProduct(ray.Direction(), ray.Direction());
        float b = 2.f * DotProduct(center_ray_origin, ray.Direction());
        float c = DotProduct(center_ray_origin, center_ray_origin) - radius * radius;
        float discriminant = b * b - 4.f * a * c;
        if (discriminant < 0.f) {
            return false;
        }
        float root = std::sqrt(discriminant);
        float q;
        if (b < 0.f) {
            q = -0.5f * (b - root);
        } else {
            q = -0.5f * (b + root);
        }
        // Find the two roots
        float t0 = q / a;
        float t1 = c / q;
        if (t0 > t1) {
            std::swap(t0, t1);
        }
        float nearest_t = t0;
        if (t0 < ray.RayMinimum()) {
            nearest_t = t1;
            if (nearest_t > ray.RayMaximum()) {
                return false;
            }
        }
        // Fill interaction data
        /* interaction->t_param = nearest_t; */
        *t_hit = nearest_t;
        interaction->hit_point = ray(nearest_t);
        interaction->normal = Normalize(interaction->hit_point - center);
        float phi = std::atan2(interaction->hit_point.z - center.z, interaction->hit_point.x - center.x);
        if (phi < 0.f) {
            phi += TWO_PI;
        }
        float theta = std::acos((interaction->hit_point.y - center.y) / radius);
        interaction->s = SSEVector(std::sin(phi), 0.f, -std::cos(phi), 0.f);
        interaction->t = SSEVector(-std::cos(theta) * std::cos(phi), -std::sin(theta), std::cos(theta) * std::sin(phi),
                                   0.f);
        interaction->u = phi / TWO_PI;
        interaction->v = theta / PI;
        /* interaction->wo = -Normalize(ray.Direction()); */

        return true;
    }

    bool Sphere::IntersectP(const Ray &ray) const {
        SSEVector center_ray_origin = ray.Origin() - center;
        // Compute terms for quadratic form
        float a = DotProduct(ray.Direction(), ray.Direction());
        float b = 2.f * DotProduct(center_ray_origin, ray.Direction());
        float c = DotProduct(center_ray_origin, center_ray_origin) - radius * radius;
        float discriminant = b * b - 4.f * a * c;
        if (discriminant < 0.f) {
            return false;
        }
        float root = std::sqrt(discriminant);
        float q;
        if (b < 0.f) {
            q = -0.5f * (b - root);
        } else {
            q = -0.5f * (b + root);
        }
        // Find the two roots
        float t0 = q / a;
        float t1 = c / q;
        if (t0 > t1) {
            std::swap(t0, t1);
        }
        float nearest_t = t0;
        if (t0 < ray.RayMinimum()) {
            nearest_t = t1;
            if (nearest_t > ray.RayMaximum()) {
                return false;
            }
        }

        return true;
    }

    float Sphere::Area() const {
        return (radius * radius * 2.f * TWO_PI);
    }

    SurfaceInteraction Sphere::Sample(float u1, float u2) const {

    }

    SurfaceInteraction Sphere::Sample(const SurfaceInteraction &from, float u1, float u2) const {

    }

    float Sphere::Pdf(const SurfaceInteraction &from, const SSEVector &wi) const {

    }

    BBox Sphere::ShapeBoundig() const {
        SSEVector radius_vector = SSEVector(radius, radius, radius, 0.f);

        return BBox(center - radius_vector, center + radius_vector);
    }


}