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
 * File:   matrix.h
 * Author: simon
 *
 * Created on October 24, 2016, 12:13 AM
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

namespace pixel {

    /*
     * Define matrix class
     */
    class matrix {
    public:
        /*
         * Constructor
         */
        matrix();

        matrix(const double m00, const double m01, const double m02, const double m03,
               const double m10, const double m11, const double m12, const double m13,
               const double m20, const double m21, const double m22, const double m23,
               const double m30, const double m31, const double m32, const double m33);

        /*
         * Transpose matrix
         */
        matrix transpose() const;

        /*
         * Direct element access
         */
        inline double operator()(uint32_t i, uint32_t j) const {
            return data[j][i];
        }

        inline double &operator()(uint32_t i, uint32_t j) {
            return data[j][i];
        }

        /*
         * Access matrix element with boundaries check
         */
        double at(uint32_t i, uint32_t j) const;

        /*
         * Set element value
         */
        void set(uint32_t i, uint32_t j, const double v);

        /*
         * Multiplication between matrices
         */
        matrix operator*(const matrix &m) const;

        /*
         * Transform direction / point
         */
        vector transform_dir(const vector &v) const;

        /*
         * Transform direction and normalize it
         */
        vector transform_normalize_dir(const vector &v) const;

        /*
         * Transform normal
         */
        vector transform_normal(const vector &n) const;

        /*
         * Transform normal and normalize it
         */
        vector transform_normalize_normal(const vector &n) const;

    private:
        /*
         * Matrix data, stored colum-wise, 128 bytes aligned
         */
        double data[4][4] __attribute__((aligned(sizeof(double) * 16)));
    };

    /*
     * Print matrix to std::cout
     */
    void print_mat(const matrix &m);

}


#endif /* MATRIX_H */

