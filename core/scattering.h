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

/* 
 * File:   scattering.h
 * Author: simon
 *
 * Created on October 30, 2016, 2:35 PM
 */

#ifndef SCATTERING_H
#define SCATTERING_H

#include "pixel.h"
#include "sse_vector.h"
#include "sse_spectrum.h"

namespace pixel {

    // Geometry inline functions for formula clarity
    // Note that this functions only work when expressing vectors in local base

    inline float CosTheta(const SSEVector &v) {
        return v.y;
    }

    inline float Cos2Theta(const SSEVector &v) {
        return (v.y * v.y);
    }

    inline float AbsCosTheta(const SSEVector &v) {
        return std::abs(v.y);
    }

    inline float Sin2Theta(const SSEVector &v) {
        return std::max(0.f, 1.f - Cos2Theta(v));
    }

    inline float SinTheta(const SSEVector &v) {
        return std::sqrt(Sin2Theta(v));
    }

    inline float CosPhi(const SSEVector &v) {
        float sin_theta = SinTheta(v);
        return (sin_theta == 0.f) ? 1.f : Clamp(v.x / sin_theta, 0.f, 1.f);
    }

    inline float SinPhi(const SSEVector &v) {
        float sin_theta = SinTheta(v);
        return (sin_theta == 0.f) ? 0.f : Clamp(v.z / sin_theta, 0.f, 1.f);
    }

    inline float Cos2Phi(const SSEVector &v) {
        return (CosPhi(v) * CosPhi(v));
    }

    inline float Sin2Phi(const SSEVector &v) {
        return (SinPhi(v) * SinPhi(v));
    }

    inline SSEVector Reflect(const SSEVector &w, const SSEVector &n) {
        return (-w + 2.f * DotProduct(w, n) * n);
    }

    inline bool Refract(const SSEVector &wi, const SSEVector &n, float eta, SSEVector *const wt) {
        float cos_theta_i = DotProduct(wi, n);
        float sin2_theta_i = std::max(0.f, 1.f - cos_theta_i * cos_theta_i);
        float sin2_theta_t = eta * eta * sin2_theta_i;

        // Check for total internal reflection
        if (sin2_theta_t >= 1.f) {
            return false;
        }
        float cos_theta_t = std::sqrt(1.f - sin2_theta_t);
        *wt = -eta * wi + (eta * cos_theta_i - cos_theta_t) * n;

        return true;
    }

    inline bool SameHemisphere(const SSEVector &v, const SSEVector &w) {
        return (v.y * w.y > 0.f);
    }

    // Define different type of BRDF
    enum BRDF_TYPE {
        // One of these two
                BRDF_REFLECTION = 1 << 0,
        BRDF_TRANSMISSION = 1 << 1,
        // Plus one of this
                BRDF_DIFFUSE = 1 << 2,
        BRDF_GLOSSY = 1 << 3,
        BRDF_SPECULAR = 1 << 4,
        ALL_BRDF = BRDF_REFLECTION | BRDF_TRANSMISSION | BRDF_DIFFUSE | BRDF_GLOSSY | BRDF_SPECULAR
    };

    // Define BSDF class, which represents a combination of multiple BRDFs
    class BSDF {
    public:
        // Constructor
        BSDF(const SurfaceInteraction &interaction);

        // Destructor
        ~BSDF();

        // Add BRDF
        void AddBRDF(const BRDF *brdf);

        // Number of matching BRDF
        uint32_t NumMatchingBRDF(BRDF_TYPE types = ALL_BRDF) const;

        // Evaluate BSDF
        SSESpectrum f(const SSEVector &wo_world, const SSEVector &wi_world, BRDF_TYPE types = ALL_BRDF) const;

        // Sample BRDF
        SSESpectrum Sample_f(const SSEVector &wo_world, SSEVector *const wi_world, float *const pdf,
                             float u1, float u2,
                             BRDF_TYPE types = ALL_BRDF,
                             BRDF_TYPE *const sampled_type = nullptr) const;

        // BSDF pdf
        float Pdf(const SSEVector &wo_world, const SSEVector &wi_world, BRDF_TYPE types = ALL_BRDF) const;

    private:
        // Transform vector to local space
        SSEVector WorldToLocal(const SSEVector &v) const {
            return SSEVector(DotProduct(v, s), DotProduct(v, geometric_normal), DotProduct(v, t), v.w);
        }

        SSEVector LocalToWorld(const SSEVector &v) const {
            return (v.x * s + v.y * geometric_normal + v.z * t);
        }

        // Local base
        const SSEVector geometric_normal;
        const SSEVector s, t;
        // Outgoing direction
        /* const SSEVector wo_local; */
        // Added BRDF
        std::vector<const BRDF *> brdfs;
    };

    // Define base BRDF class

    class BRDF {
    public:
        // Constructor
        BRDF(const BRDF_TYPE type);

        // Destructor
        virtual ~BRDF();

        // Check if BRDF matches given type
        bool MatchesTypes(const BRDF_TYPE types) const;

        // Evaluate BRDF
        virtual SSESpectrum f(const SSEVector &wo, const SSEVector &wi) const = 0;

        // Sample BRDF
        virtual SSESpectrum Sample_f(const SSEVector &wo, SSEVector *const wi,
                                     float *const pdf, float u1, float u2,
                                     BRDF_TYPE *const sampled_type = nullptr) const;

        // Pdf of the BRDF
        virtual float Pdf(const SSEVector &wo, const SSEVector &wi) const;

        // BRDF type
        const BRDF_TYPE type;
    };

    // Lambertian BRDF

    class LambertianReflectionBRDF : public BRDF {
    public:
        // Constructor
        LambertianReflectionBRDF(const SSESpectrum &r);

        SSESpectrum f(const SSEVector &wo, const SSEVector &wi) const override;

    private:
        // Reflectance
        const SSESpectrum rho;
    };


}

#endif /* SCATTERING_H */

