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

#include "scattering.h"
#include "interaction.h"
#include "montecarlo.h"
#include <cassert>

namespace pixel {

    BSDF::BSDF(const SurfaceInteraction &interaction)
            : geometric_normal(interaction.normal), s(interaction.s), t(interaction.t),
            /* wo_local(WorldToLocal(interaction.wo)),*/ brdfs() {
    }

    BSDF::~BSDF() {
        for (auto brdf : brdfs) {
            delete brdf;
        }
    }

    void BSDF::AddBRDF(const BRDF *brdf) {
        brdfs.push_back(brdf);
    }

    uint32_t BSDF::NumMatchingBRDF(BRDF_TYPE types) const {
        uint32_t num = 0;
        for (auto brdf : brdfs) {
            if (brdf->MatchesTypes(types)) {
                ++num;
            }
        }

        return num;
    }

    SSESpectrum BSDF::f(const SSEVector &wo_world, const SSEVector &wi_world, BRDF_TYPE types) const {
        SSEVector wo_local = WorldToLocal(wo_world);
        SSEVector wi_local = WorldToLocal(wi_world);
        if (wo_local.y == 0.f) {
            return SSESpectrum();
        }
        SSESpectrum f(0.f);
        for (auto brdf : brdfs) {
            if (brdf->MatchesTypes(types) &&
                ((SameHemisphere(wi_local, wo_local) && (brdf->type & BRDF_REFLECTION)) ||
                 (!SameHemisphere(wi_local, wo_local) && (brdf->type & BRDF_TRANSMISSION)))) {
                f += brdf->f(wo_local, wi_local);
            }
        }

        return f;
    }

    SSESpectrum BSDF::Sample_f(const SSEVector &wo_world, SSEVector *const wi_world, float *const pdf,
                               float u1, float u2, BRDF_TYPE types, BRDF_TYPE *const sampled_type) const {
        // Choose BRDF to sample
        uint32_t matching_brdf = NumMatchingBRDF(types);
        if (matching_brdf == 0) {
            *pdf = 0.f;
            if (sampled_type) {
                *sampled_type = BRDF_TYPE(0);
            }
            return SSESpectrum(0.f);
        }
        // Choose component
        uint32_t sampled_comp = FMin(static_cast<uint32_t> (u1 * matching_brdf), matching_brdf - 1);

        // Get pointer to chosen BRDF
        const BRDF *sampled_brdf = nullptr;
        uint32_t count = sampled_comp;
        for (auto brdf : brdfs) {
            if (brdf->MatchesTypes(types) && count-- == 0) {
                sampled_brdf = brdf;
                break;
            }
        }
#ifdef DEBUG
        assert(sampled_brdf != nullptr);
#endif

        // Sample the chosen BRDF
        SSEVector wo_local = WorldToLocal(wo_world);
        SSEVector wi_local;
        if (wo_local.y == 0.f) {
            return SSESpectrum();
        }
        *pdf = 0.f;
        if (sampled_type) {
            *sampled_type = sampled_brdf->type;
        }
        // Sample the BRDF
        if (sampled_type) { *sampled_type = sampled_brdf->type; }
        SSESpectrum f = sampled_brdf->Sample_f(wo_local, &wi_local, pdf, u1, u2, sampled_type);
        if (*pdf == 0.f) {
            if (sampled_type) {
                *sampled_type = BRDF_TYPE(0);
            }
            return SSESpectrum(0.f);
        }
        *wi_world = LocalToWorld(wi_local);
        // Compute overall PDF for all matching BRDFs, unless specular
        if (!(sampled_brdf->type & BRDF_SPECULAR) && matching_brdf > 1) {
            for (auto brdf : brdfs) {
                if (brdf != sampled_brdf && brdf->MatchesTypes(types)) {
                    *pdf += brdf->Pdf(wo_local, wi_local);
                }
            }
        }
        if (matching_brdf > 1) {
            *pdf /= static_cast<float> (matching_brdf);
        }

        // Compute value of BSDF for sampled direction, unless specular
        if (!(sampled_brdf->type & BRDF_SPECULAR) && matching_brdf > 1) {
            f = SSESpectrum(0.f);
            for (auto brdf : brdfs) {
                if (brdf->MatchesTypes(types) &&
                    ((SameHemisphere(wi_local, wo_local) && (brdf->type & BRDF_REFLECTION)) ||
                     (!SameHemisphere(wi_local, wo_local) && (brdf->type & BRDF_TRANSMISSION)))) {
                    f += brdf->f(wo_local, wi_local);
                }
            }
        }

        return f;
    }

    float BSDF::Pdf(const SSEVector &wo_world, const SSEVector &wi_world, BRDF_TYPE types) const {
        if (brdfs.size() == 0) {
            return 0.f;
        }
        SSEVector wo_local = WorldToLocal(wo_world);
        SSEVector wi_local = WorldToLocal(wi_world);
        if (wo_local.y == 0.f) {
            return 0.f;
        }
        float pdf = 0.f;
        uint32_t matching_brdf = 0;
        for (auto brdf : brdfs) {
            if (brdf->MatchesTypes(types)) {
                matching_brdf++;
                pdf += brdf->Pdf(wo_local, wi_local);
            }
        }

        return matching_brdf > 0 ? (pdf / static_cast<float> (matching_brdf)) : 0.f;
    }

    BRDF::BRDF(const BRDF_TYPE type)
            : type(type) {
    }

    BRDF::~BRDF() {
    }

    bool BRDF::MatchesTypes(const BRDF_TYPE types) const {
        return ((type & types) == type);
    }

    SSESpectrum BRDF::Sample_f(const SSEVector &wo, SSEVector *const wi, float *const pdf,
                               float u1, float u2, BRDF_TYPE *const sampled_type) const {
        // Cosine sample the hemisphere
        *wi = CosineSampleHemisphere(u1, u2);
        if (wo.y < 0.f) {
            wi->y *= -1.f;
        }
        *pdf = Pdf(wo, *wi);

        return f(wo, *wi);
    }

    float BRDF::Pdf(const SSEVector &wo, const SSEVector &wi) const {
        return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * ONE_OVER_PI : 0.f;
    }

    LambertianReflectionBRDF::LambertianReflectionBRDF(const SSESpectrum &r)
            : BRDF(BRDF_TYPE(BRDF_REFLECTION | BRDF_DIFFUSE)), rho(r) {
    }

    SSESpectrum LambertianReflectionBRDF::f(const SSEVector &wo, const SSEVector &wi) const {
        return (rho * ONE_OVER_PI);
    }


}