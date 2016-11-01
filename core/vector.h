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
 * File:   vector.h
 * Author: simon
 *
 * Created on October 21, 2016, 3:53 PM
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "pixel.h"
#include <cmath>
#include <immintrin.h>

namespace pixel {

    /*
     * Define 4D-vector struct
     */
    struct vector {

        /*
         * Constructor
         */
        vector() {
            e[0] = e[1] = e[2] = e[3] = 0.0;
        }

        vector(const double x, const double y, const double z, const double w) {
            e[0] = x;
            e[1] = y;
            e[2] = z;
            e[3] = w;
        }

        /*
         * Vector elements, 32 bytes aligned
         */
        double e[4] __attribute__((aligned(sizeof(double) * 4)));
    };

    /*
     * Define inline functions for vectors
     */

    /*
     * Print vector
     */
    inline void print_vec(const vector &v) {
        std::cout << "[" << v.e[0] << "," << v.e[1] << "," << v.e[2] << "," << v.e[3] << "]" << std::endl;
    }

    /*
     * Dot product
     */
    inline double dot(const vector &v1, const vector &v2) {
        // Load into two registers the two vector data
        __m256d a = _mm256_load_pd(v1.e);
        __m256d b = _mm256_load_pd(v2.e);
        // Multiply the two together and store result in a
        a = _mm256_mul_pd(a, b);
        // Store result
        double res[4];
        _mm256_store_pd(res, a);

        // Return sum over components
        return (res[0] + res[1] + res[2] + res[3]);
    }

    /*
     * Absolute value of the dot product
     */
    inline double abs_dot(const vector &v1, const vector &v2) {
        return std::abs(dot(v1, v2));
    }

    /*
     * Squared length of a vector
     */
    inline double sqrd_length(const vector &v) {
        return dot(v, v);
    }

    /*
     * Length of a vector
     */
    inline double length(const vector &v) {
        return (std::sqrt(sqrd_length(v)));
    }

    /*
     * Normalize vector
     */
    inline vector normalize(const vector &v) {
        // Load into register the vector data
        __m256d a = _mm256_load_pd(v.e);
        // Multiply with itself
        __m256d b = _mm256_mul_pd(a, a);
        // Store result
        double res[4];
        _mm256_store_pd(res, b);
        // Compute length
        double norm = std::sqrt(res[0] + res[1] + res[2] + res[3]);
        // Compute result
        b = _mm256_div_pd(a, _mm256_set1_pd(norm));
        // Create result
        vector result;
        // Set result
        _mm256_store_pd(result.e, b);

        return result;
    }

    inline void normalize(vector *const v) {
        // Load into register the vector data
        __m256d a = _mm256_load_pd(v->e);
        // Multiply with itself
        __m256d b = _mm256_mul_pd(a, a);
        // Store result
        double res[4];
        _mm256_store_pd(res, b);
        // Compute length
        double norm = std::sqrt(res[0] + res[1] + res[2] + res[3]);
        // Compute result
        b = _mm256_div_pd(a, _mm256_set1_pd(norm));
        // Set result
        _mm256_store_pd(v->e, b);
    }

    /*
     * Compute cross product
     * Considers only the first three component and set last to 0)
     */
    inline vector cross(const vector &v1, const vector &v2) {
        return vector(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                      v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                      v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0],
                      0.0);
    }

    /*
     * Compute orthogonal vector to the given one
     */
    inline vector orthogonal(const vector &v) {
        if (std::abs(v.e[0]) <= std::abs(v.e[1]) && std::abs(v.e[0]) <= std::abs(v.e[2])) {
            return vector(0, -v.e[2], v.e[1], 0);
        } else if (std::abs(v.e[1]) <= std::abs(v.e[0]) && std::abs(v.e[1]) <= std::abs(v.e[2])) {
            return vector(-v.e[2], 0, v.e[0], 0);
        } else {
            return vector(-v.e[1], v.e[0], 0, 0);
        }
    }
}

#endif /* VECTOR_H */

