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
 * File:   sse_spectrum.h
 * Author: simon
 *
 * Created on October 26, 2016, 10:38 PM
 */

#ifndef SSE_SPECTRUM_H
#define SSE_SPECTRUM_H

#include "pixel.h"
#include <immintrin.h>

namespace pixel {

    class SSESpectrum {
    public:
        // Constructor
        SSESpectrum()
                : r(0.f), g(0.f), b(0.f), w(0.f) {
        }

        SSESpectrum(const SSESpectrum &other)
                : xmm(other.xmm) {
        }

        SSESpectrum(float r, float g, float b)
                : r(r), g(g), b(b), w(0.f) {
        }

        explicit SSESpectrum(float v)
                : r(v), g(v), b(v), w(0.f) {
        }

        SSESpectrum(const __m128 &xmm)
                : xmm(xmm) {
        }

        // Assignment operator
        inline SSESpectrum &operator=(const SSESpectrum &other) {
            if (this != &other) {
                xmm = other.xmm;
            }

            return *this;
        }

        inline SSESpectrum &operator=(const __m128 &other) {
            xmm = other;

            return *this;
        }

        // Operator on self
        void operator+=(const SSESpectrum &s) {
            xmm = _mm_add_ps(xmm, s.xmm);
        }

        void operator+=(const __m128 &xmm1) {
            xmm = _mm_add_ps(xmm, xmm1);
        }

        void operator-=(const SSESpectrum &s) {
            xmm = _mm_sub_ps(xmm, s.xmm);
        }

        void operator-=(const __m128 &xmm1) {
            xmm = _mm_sub_ps(xmm, xmm1);
        }

        void operator*=(float s) {
            xmm = _mm_mul_ps(xmm, _mm_set1_ps(s));
        }

        void operator*=(const SSESpectrum &s) {
            xmm = _mm_mul_ps(xmm, s.xmm);
        }

        void operator*=(const __m128 &xmm1) {
            xmm = _mm_mul_ps(xmm, xmm1);
        }

        void operator/=(float s) {
            xmm = _mm_div_ps(xmm, _mm_set1_ps(s));
        }

        union {
            // Packed vector datatype for SSE
            __m128 xmm;
            // Named vector fields

            struct {
                float r, g, b, w;
            };
        };
    };

    // Sum
    inline __m128 operator+(const SSESpectrum &s1, const SSESpectrum &s2) {
        return _mm_add_ps(s1.xmm, s2.xmm);
    }

    inline __m128 operator+(const __m128 &xmm1, const SSESpectrum &s) {
        return _mm_add_ps(xmm1, s.xmm);
    }

    // Subtraction
    inline __m128 operator-(const SSESpectrum &s1, const SSESpectrum &s2) {
        return _mm_sub_ps(s1.xmm, s2.xmm);
    }

    inline __m128 operator-(const __m128 &xmm, const SSESpectrum &s) {
        return _mm_sub_ps(xmm, s.xmm);
    }

    // Multiplication
    inline __m128 operator*(const SSESpectrum &s1, const SSESpectrum &s2) {
        return _mm_mul_ps(s1.xmm, s2.xmm);
    }

    inline __m128 operator*(const __m128 &xmm, const SSESpectrum &s) {
        return _mm_mul_ps(xmm, s.xmm);
    }

    // Spectrum scaling
    inline __m128 operator*(float t, const SSESpectrum &s) {
        return _mm_mul_ps(_mm_set1_ps(t), s.xmm);
    }

    inline __m128 operator*(const SSESpectrum &s, float t) {
        return _mm_mul_ps(_mm_set1_ps(t), s.xmm);
    }

    inline __m128 operator/(const SSESpectrum &s, float t) {
        return _mm_div_ps(s.xmm, _mm_set1_ps(t));
    }

    // Check if spectrum is black
    inline bool IsBlack(const SSESpectrum &s) {
        return (s.r == 0.f && s.g == 0.f && s.b == 0.f);
    }

    // Spectrum power function
    inline SSESpectrum Pow(const SSESpectrum &s, float e) {
        return SSESpectrum(std::pow(s.r, e), std::pow(s.g, e), std::pow(s.b, e));
    }

    // Print spectrum
    inline void PrintSSESpectrum(const SSESpectrum &s) {
        std::cout << "[" << s.r << ", " << s.g << ", " << s.b << "]" << std::endl;
    }

    // Clamp color
    inline SSESpectrum Clamp(const SSESpectrum &s, float min, float max) {
        return SSESpectrum(Clamp(s.r, min, max), Clamp(s.g, min, max), Clamp(s.b, min, max));
    }

}

#endif /* SSE_SPECTRUM_H */

