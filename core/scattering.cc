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

    float FrDielectric(float cos_theta_i, float eta_i, float eta_t) {
        cos_theta_i = Clamp(cos_theta_i, -1.f, 1.f);
        // Check if we are entering
        bool entering = cos_theta_i > 0.f;
        if (!entering) {
            std::swap(eta_i, eta_t);
            cos_theta_i = std::abs(cos_theta_i);
        }
        // Compute cos_theta_t using Snell's law
        float sin_theta_i = std::sqrt(std::max(0.f, 1.f - cos_theta_i * cos_theta_i));
        float sin_theta_t = eta_i / eta_t * sin_theta_i;
        // Handle total internal reflection
        if (sin_theta_t >= 1.f) {
            return 1.f;
        }
        float cos_theta_t = std::sqrt(std::max(0.f, 1.f - sin_theta_t * sin_theta_t));
        float r_parl = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) /
                       ((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
        float r_perp = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) /
                       ((eta_i * cos_theta_i) + (eta_t * cos_theta_t));

        return (r_parl * r_parl + r_perp * r_perp) * 0.5f;
    }

    FresnelInterface::~FresnelInterface() {
    }

    FresnelDielectric::FresnelDielectric(float eta_i, float eta_t)
            : eta_i(eta_i), eta_t(eta_t) {
    }

    SSESpectrum FresnelDielectric::Evaluate(float cos_theta_i) const {
        return SSESpectrum(FrDielectric(cos_theta_i, eta_i, eta_t));
    }

    SSESpectrum FresnelIdeal::Evaluate(float) const {
        return SSESpectrum(1.f);
    }

    BSDF::BSDF(const SurfaceInteraction &interaction)
            : geometric_normal(interaction.normal), s(interaction.s), t(interaction.t), brdfs() {
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
        SSESpectrum f;
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

    bool BRDF::MatchesTypes(BRDF_TYPE types) const {
        if (type <= types) {
            return ((type & types) == type);
        } else {
            return ((type & types) == types);
        }
    }

    SSESpectrum BRDF::Sample_f(const SSEVector &wo, SSEVector *const wi, float *const pdf,
                               float u1, float u2, BRDF_TYPE *const) const {
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

    LambertianReflection::LambertianReflection(const SSESpectrum &r)
            : BRDF(BRDF_TYPE(BRDF_REFLECTION | BRDF_DIFFUSE)), rho(r) {
    }

    SSESpectrum LambertianReflection::f(const SSEVector &, const SSEVector &) const {
        return SSESpectrum(rho * ONE_OVER_PI);
    }

    SpecularReflection::SpecularReflection(const SSESpectrum &R, FresnelInterface *const f)
            : BRDF(BRDF_TYPE(BRDF_REFLECTION | BRDF_SPECULAR)), R(R), fresnel(f) {
    }

    SpecularReflection::~SpecularReflection() {
        delete fresnel;
    }

    SSESpectrum SpecularReflection::f(const SSEVector &, const SSEVector &) const {
        return SSESpectrum(0.f);
    }

    SSESpectrum SpecularReflection::Sample_f(const SSEVector &wo, SSEVector *const wi,
                                             float *const pdf, float, float,
                                             BRDF_TYPE *const) const {
        // Compute perfect specular direction
        *wi = SSEVector(-wo.x, wo.y, -wo.z, 0.f);
        *pdf = 1.f;

        // Note that there is not division by cosine term!
        return SSESpectrum(fresnel->Evaluate(CosTheta(*wi)) * R);
    }

    float SpecularReflection::Pdf(const SSEVector &, const SSEVector &) const {
        return 0.f;
    }

    SpecularTransmission::SpecularTransmission(const SSESpectrum &T, float eta_a, float eta_b)
            : BRDF(BRDF_TYPE(BRDF_TRANSMISSION | BRDF_SPECULAR)),
              T(T), eta_a(eta_a), eta_b(eta_b),
              fresnel(eta_a, eta_b) {
    }

    SSESpectrum SpecularTransmission::f(const SSEVector &, const SSEVector &) const {
        return SSESpectrum(0.f);
    }

    SSESpectrum SpecularTransmission::Sample_f(const SSEVector &wo, SSEVector *const wi,
                                               float *const pdf, float, float,
                                               BRDF_TYPE *const) const {
        // Find out if we are entering or exiting
        bool entering = CosTheta(wo) > 0.f;
        float eta_i = entering ? eta_a : eta_b;
        float eta_t = entering ? eta_b : eta_a;

        // Find normal
        SSEVector normal = entering ? SSEVector(0.f, 1.f, 0.f, 0.f) : SSEVector(0.f, -1.f, 0.f, 0.f);
        // Check if we have total internal refraction
        if (!Refract(wo, normal, eta_i / eta_t, wi)) {
            return SSESpectrum(0.f);
        }
        *pdf = 1.f;

        return SSESpectrum(
                (eta_i * eta_i) / (eta_t * eta_t) * T * (SSESpectrum(1.f) - fresnel.Evaluate(CosTheta(*wi))));
    }

    float SpecularTransmission::Pdf(const SSEVector &, const SSEVector &) const {
        return 0.f;
    }

    OrenNayar::OrenNayar(const SSESpectrum &r, float sigma)
    : BRDF(BRDF_TYPE(BRDF_REFLECTION | BRDF_DIFFUSE)), rho(r) {
        sigma = DegToRad(sigma);
        float sigma_2 = sigma * sigma;
        A = 1.f - (sigma_2 / (2.f * (sigma_2 + 0.33f)));
        B = 0.45f * sigma_2 / (sigma_2 + 0.09f);
    }

    SSESpectrum OrenNayar::f(const SSEVector &wo, const SSEVector &wi) const {
        float sin_theta_i = SinTheta(wi);
        float sin_theta_o = SinTheta(wo);
        float max_cos = 0.f;
        if (sin_theta_i > EPS && sin_theta_o > EPS) {
            float sin_phi_i =SinPhi(wi), cos_phi_i = CosPhi(wi);
            float sin_phi_o =SinPhi(wo), cos_phi_o = CosPhi(wo);
            float d_cos = cos_phi_i * cos_phi_o + sin_phi_i * sin_phi_o;
            max_cos = FMax(0.f, d_cos);
        }

        // Compute sine and tangent terms or Oren Nayar
        float sin_alpha, tan_beta;
        if (AbsCosTheta(wi) > AbsCosTheta(wo)) {
            sin_alpha = sin_theta_o;
            tan_beta = sin_theta_i / AbsCosTheta(wi);
        } else {
            sin_alpha = sin_theta_i;
            tan_beta = sin_theta_o / AbsCosTheta(wo);
        }

        return SSESpectrum(rho * ONE_OVER_PI * (A + B * max_cos * sin_alpha * tan_beta));
    }

//    FresnelSpecular::FresnelSpecular(const SSESpectrum &R, const SSESpectrum &T, float eta_a, float eta_b)
//            : BRDF(BRDF_TYPE(BRDF_REFLECTION | BRDF_TRANSMISSION | BRDF_SPECULAR)),
//              R(R), T(T),
//              eta_a(eta_a), eta_b(eta_b),
//              fresnel(eta_a, eta_b) {
//    }
//
//    SSESpectrum FresnelSpecular::f(const SSEVector &, const SSEVector &) const {
//        return SSESpectrum(0.f);
//    }
//
//    SSESpectrum FresnelSpecular::Sample_f(const SSEVector &wo, SSEVector *const wi,
//                                          float *const pdf, float u1, float,
//                                          BRDF_TYPE *const sampled_type) const {
//        // Find out if we are entering or exiting
//        bool entering = CosTheta(wo) > 0.f;
//        float eta_i = entering ? eta_a : eta_b;
//        float eta_t = entering ? eta_b : eta_a;
//        // Find normal
//        SSEVector normal = entering ? SSEVector(0.f, 1.f, 0.f, 0.f) : SSEVector(0.f, -1.f, 0.f, 0.f);
//
//        // Check for refraction
//        if (!Refract(wo, normal, eta_i / eta_t, wi)) {
//            // Reflect ray
//            *wi = SSEVector(-wo.x, wo.y, -wo.z, 0.f);
//            *pdf = 1.f;
//            if (sampled_type) {
//                *sampled_type = BRDF_TYPE(BRDF_SPECULAR | BRDF_REFLECTION);
//            }
//
//            return R;
//        }
//
//        // Evaluate Fresnel term
//        float F = FrDielectric(CosTheta(wo), eta_i, eta_t);
//        if (u1 < F) {
//            // Compute perfect specular direction
//            *wi = SSEVector(-wo.x, wo.y, -wo.z, 0.f);
//            *pdf = F;
//            if (sampled_type) {
//                *sampled_type = BRDF_TYPE(BRDF_SPECULAR | BRDF_REFLECTION);
//            }
//
//            return (F * R);
//        } else {
//            // Find normal
////            SSEVector normal = entering ? SSEVector(0.f, 1.f, 0.f, 0.f) : SSEVector(0.f, -1.f, 0.f, 0.f);
//            // Check if we have total internal refraction
//            if (!Refract(wo, normal, eta_i / eta_t, wi)) {
//                return SSESpectrum(0.f);
//            }
//            *pdf = 1.f - F;
//            if (sampled_type) {
//                *sampled_type = BRDF_TYPE(BRDF_SPECULAR | BRDF_TRANSMISSION);
//            }
//
//            return (eta_i * eta_i) / (eta_t * eta_t) * T * (SSESpectrum(1.f) - fresnel.Evaluate(CosTheta(*wi)));
//        }
//    }
//
//    float FresnelSpecular::Pdf(const SSEVector &, const SSEVector &) const {
//        return 0.f;
//    }
}
