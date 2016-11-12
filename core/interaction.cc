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

#include "interaction.h"
#include "ray.h"
#include "material.h"

namespace pixel {

    SurfaceInteraction::SurfaceInteraction()
            : hit_point(), normal(), s(), t(), u(0.f), v(0.f), prim_ptr(nullptr), mat_ptr(nullptr), bsdf(nullptr) {}

//    SurfaceInteraction::~SurfaceInteraction() {
//        delete bsdf;
//    }

    SurfaceInteraction::SurfaceInteraction(const SSEVector &hit, const SSEVector &n,
                                           const SSEVector &s, const SSEVector &t, float u, float v,
                                           const PrimitiveInterface *prim_ptr,
                                           const MaterialInterface *const mat_ptr)
            : hit_point(hit), normal(n), s(s), t(t), u(u), v(v),
              prim_ptr(prim_ptr), mat_ptr(mat_ptr), bsdf(nullptr) {
    }

    SSESpectrum SurfaceInteraction::EmittedRadiance(const SSEVector &w) const {
        return (mat_ptr->type == MAT_EMITTING) ? mat_ptr->Emission(*this, w) : SSESpectrum(0.f);
    }

    Ray SurfaceInteraction::SpawnRay(const SSEVector &dir, uint32_t depth) const {
        return Ray(hit_point, dir, EPS, INFINITY, depth);
    }

    void SurfaceInteraction::GenerateBSDF() {
        bsdf = mat_ptr->GetBSDF(*this);
    }

    void TransformSurfaceInteraction(SurfaceInteraction *const interaction, const SSEMatrix &mat) {
        // Transform hit_point
        interaction->hit_point = mat * interaction->hit_point;
        // Transform normal
        interaction->normal = Transpose(Inverse(mat)) * interaction->normal;
        // Small correction we need to consider or normalization will be incorrect
        interaction->normal.w = 0.f;
        Normalize(&(interaction->normal));
        // Transform tangent space
        interaction->s = Normalize(mat * interaction->s);
        interaction->t = Normalize(mat * interaction->t);
    }

}
