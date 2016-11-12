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

#include <montecarlo.h>
#include "sphere.h"
#include "ray.h"
#include "interaction.h"
#include "bbox.h"

namespace pixel {

    Sphere::Sphere(const SSEMatrix &l2w, float r)
            : ShapeInterface(l2w), radius(r) {
    }

    bool Sphere::Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);
        // Compute terms for quadratic form
        float a = DotProduct(local_ray.Direction(), local_ray.Direction());
        float b = 2.f * DotProduct(local_ray.Origin(), local_ray.Direction());
        float c = DotProduct3(local_ray.Origin(), local_ray.Origin()) - radius * radius;
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
        if (t0 > local_ray.RayMaximum() || t1 < local_ray.RayMinimum()) {
            return false;
        }
        float nearest_t = t0;
        if (t0 < local_ray.RayMinimum()) {
            nearest_t = t1;
            if (nearest_t > local_ray.RayMaximum()) {
                return false;
            }
        }
        // Fill interaction data
        *t_hit = nearest_t;
        interaction->hit_point = local_ray(nearest_t);
        interaction->normal = Normalize(interaction->hit_point - SSEVector(0.f, 0.f, 0.f, 1.f));
        float phi = std::atan2(interaction->hit_point.z, interaction->hit_point.x);
        if (phi < 0.f) {
            phi += TWO_PI;
        }
        float theta = std::acos((interaction->hit_point.y) / radius);
        if (theta > 0.f) {
            interaction->s = SSEVector(std::sin(phi), 0.f, -std::cos(phi), 0.f);
            interaction->t = SSEVector(std::cos(theta) * std::cos(phi), -std::sin(theta),
                                       std::cos(theta) * std::sin(phi),
                                       0.f);
        } else {
            CoordinateSystem(interaction->normal, &(interaction->s), &(interaction->t));
        }
        interaction->u = phi / TWO_PI;
        interaction->v = theta / PI;

        // Transform interaction back to world space
        TransformSurfaceInteraction(interaction, local_to_world);

        return true;
    }

    bool Sphere::IntersectP(const Ray &ray) const {
        // Transform ray to local space
        Ray local_ray = TransformRay(ray, world_to_local);
        // Compute terms for quadratic form
        float a = DotProduct(local_ray.Direction(), local_ray.Direction());
        float b = 2.f * DotProduct(local_ray.Origin(), local_ray.Direction());
        float c = DotProduct3(local_ray.Origin(), local_ray.Origin()) - radius * radius;
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
        if (t0 > local_ray.RayMaximum() || t1 < local_ray.RayMinimum()) {
            return false;
        }
        if (t0 < local_ray.RayMinimum()) {
            if (t1 > local_ray.RayMaximum()) {
                return false;
            }
        }

        return true;
    }

    float Sphere::Area() const {
        return (radius * radius * 2.f * TWO_PI);
    }

    SurfaceInteraction Sphere::Sample(float u1, float u2) const {
        SurfaceInteraction interaction;
        // Sample point on sphere
        SSEVector p_sphere = radius * UniformSampleSphere(u1, u2);
        interaction.hit_point = local_to_world * SSEVector(p_sphere.x, p_sphere.y, p_sphere.z, 1.f);
        interaction.normal = Normalize(Transpose(world_to_local) * p_sphere);

        return interaction;
    }

    SurfaceInteraction Sphere::Sample(const SurfaceInteraction &from, float u1, float u2) const {
        // Compute center in world space
        SSEVector sphere_center = local_to_world * SSEVector(0.f, 0.f, 0.f, 1.f);
        // Create coordinate system for sphere sampling
        SSEVector v_c = Normalize(sphere_center - from.hit_point);
        SSEVector u_c, w_c;
        CoordinateSystem(v_c, &u_c, &w_c);

        // Sample sphere into visible cone
        float sin_theta_max_2 = radius * radius / SqrdLength(from.hit_point - sphere_center);
        float cos_theta_max = std::sqrt(std::max(0.f, 1.f - sin_theta_max_2));
        float cos_theta = (1.f - u1) + u1 * cos_theta_max;
        float sin_theta = std::sqrt(std::max(0.f, 1.f - cos_theta * cos_theta));
        float phi = u2 * TWO_PI;

        // Compute angle alpha between sphere center and sampled point
        float dc = Length(from.hit_point - sphere_center);
        float ds = dc * cos_theta - std::sqrt(std::max(0.f, radius * radius - dc * dc * sin_theta * sin_theta));
        float cos_alpha = (dc * dc + radius * radius - ds * ds) / (2.f * dc * radius);
        float sin_alpha = std::sqrt(std::max(0.f, 1.f - cos_alpha * cos_alpha));

        SurfaceInteraction interaction;
        interaction.normal = SphericalDirection(sin_alpha, cos_alpha, phi, -u_c, -v_c, -w_c);
        interaction.hit_point = SSEVector(0.f, 0.f, 0.f, 1.f) + radius * interaction.normal;

        TransformSurfaceInteraction(&interaction, local_to_world);

        return interaction;
    }

    float Sphere::Pdf(const SurfaceInteraction &from, const SSEVector &) const {
        // Compute center in world space
        SSEVector sphere_center = local_to_world * SSEVector(0.f, 0.f, 0.f, 1.f);
        float sin_theta_max_2 = radius * radius / SqrdLength(from.hit_point - sphere_center);
        float cos_theta_max = std::sqrt(std::max(0.f, 1.f - sin_theta_max_2));

        return UniformPdfCone(cos_theta_max);
    }

    BBox Sphere::ShapeBounding() const {
        SSEVector radius_vector = SSEVector(radius, radius, radius, 0.f);

        return BBox(-radius_vector, radius_vector);
    }


}
