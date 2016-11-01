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
 * File:   spectrum.h
 * Author: simon
 *
 * Created on October 22, 2016, 2:35 AM
 */

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "pixel.h"
#include "immintrin.h"

namespace pixel {

    /*
     * Define spectrum type
     */
    struct spectrum {

        /*
         * Constructor
         */
        spectrum() {
            e[0] = e[1] = e[2] = e[3] = 0.0;
        }

        spectrum(const double v) {
            e[0] = e[1] = e[2] = v;
            e[3] = 0.0;
        }

        spectrum(const double r, const double g, const double b) {
            e[0] = r;
            e[1] = g;
            e[2] = b;
            e[3] = 0.0;
        }

        /*
         * Spectrum elements, 32 bytes aligned
         */
        double e[4] __attribute__((aligned(sizeof(double) * 4)));
    };

    /*
     * Define inline functions for spectrum
     */

    /*
     * Print spectrum
     */
    inline void print_spectrum(const spectrum &s) {
        std::cout << "(" << s.e[0] << "," << s.e[1] << "," << s.e[2] << ")" << std::endl;
    }

    /*
     * Check if a color is black
     */
    inline bool is_black(const spectrum &s) {
        return (s.e[0] == 0.0 && s.e[1] == 0.0 && s.e[2] == 0.0);
    }

    /*
     * Power function for spectrum
     */
    inline spectrum pow(const spectrum &s, const double e) {
        //        // Load color data into __256d
        //        __m256d a = _mm256_load_pd(s.e);
        //        // Compute pow function
        //        a = _mm256_pow_pd(a, _mm256_set_pd(e, e, e, 0));
        return spectrum(std::pow(s.e[0], e), std::pow(s.e[1], e), std::pow(s.e[2], e));
    }

    /*
     * Clamp color between two values
     */
    inline spectrum clamp(const spectrum &s, const double min, const double max) {
        return spectrum(clamp(s.e[0], min, max), clamp(s.e[1], min, max), clamp(s.e[2], min, max));
    }
}

#endif /* SPECTRUM_H */

