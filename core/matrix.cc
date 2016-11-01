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

#include "matrix.h"
#include <cstring>
#include <cassert>

namespace pixel {

    matrix::matrix() {
        for (uint32_t i = 0; i < 4; i++) {
            for (uint32_t j = 0; j < 4; j++) {
                if (i == j) {
                    data[j][i] = 1.0;
                } else {
                    data[j][i] = 0.0;
                }
            }
        }
    }

    matrix::matrix(const double m00, const double m01, const double m02, const double m03,
                   const double m10, const double m11, const double m12, const double m13,
                   const double m20, const double m21, const double m22, const double m23,
                   const double m30, const double m31, const double m32, const double m33) {
        data[0][0] = m00;
        data[1][0] = m01;
        data[2][0] = m02;
        data[3][0] = m03;
        data[0][1] = m10;
        data[1][1] = m11;
        data[2][1] = m12;
        data[3][1] = m13;
        data[0][2] = m20;
        data[1][2] = m21;
        data[2][2] = m22;
        data[3][2] = m23;
        data[0][3] = m30;
        data[1][3] = m31;
        data[2][3] = m32;
        data[3][3] = m33;
    }

    matrix matrix::transpose() const {
        matrix transposed;

        for (uint32_t i = 0; i < 4; i++) {
            for (uint32_t j = 0; j < 4; j++) {
                transposed.data[i][j] = data[j][i];
            }
        }

        return transposed;
    }

    double matrix::at(uint32_t i, uint32_t j) const {
        assert(i <= 4);
        assert(j <= 4);

        return data[j][i];
    }

    void matrix::set(uint32_t i, uint32_t j, const double v) {
        assert(i <= 4);
        assert(j <= 4);

        data[j][i] = v;
    }

    matrix matrix::operator*(const matrix &m) const {
        matrix result;

        // Load the columns of this matrix
        __m256d col_0 = _mm256_load_pd(data[0]);
        __m256d col_1 = _mm256_load_pd(data[1]);
        __m256d col_2 = _mm256_load_pd(data[2]);
        __m256d col_3 = _mm256_load_pd(data[3]);

        // Create variable to hold current computed column
        __m256d a = _mm256_mul_pd(col_0, _mm256_set1_pd(m.data[0][0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_1, _mm256_set1_pd(m.data[0][1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_2, _mm256_set1_pd(m.data[0][2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_3, _mm256_set1_pd(m.data[0][3])));
        // Set first colum
        _mm256_store_pd(result.data[0], a);

        a = _mm256_mul_pd(col_0, _mm256_set1_pd(m.data[1][0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_1, _mm256_set1_pd(m.data[1][1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_2, _mm256_set1_pd(m.data[1][2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_3, _mm256_set1_pd(m.data[1][3])));
        // Set second colum
        _mm256_store_pd(result.data[1], a);

        a = _mm256_mul_pd(col_0, _mm256_set1_pd(m.data[2][0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_1, _mm256_set1_pd(m.data[2][1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_2, _mm256_set1_pd(m.data[2][2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_3, _mm256_set1_pd(m.data[2][3])));
        // Set third colum
        _mm256_store_pd(result.data[2], a);

        a = _mm256_mul_pd(col_0, _mm256_set1_pd(m.data[3][0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_1, _mm256_set1_pd(m.data[3][1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_2, _mm256_set1_pd(m.data[3][2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(col_3, _mm256_set1_pd(m.data[3][3])));
        // Set fourth colum
        _mm256_store_pd(result.data[3], a);

        return result;
    }

    vector matrix::transform_dir(const vector &v) const {
        // Multiply with first vector component
        __m256d a = _mm256_mul_pd(_mm256_load_pd(data[0]), _mm256_set1_pd(v.e[0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[1]), _mm256_set1_pd(v.e[1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[2]), _mm256_set1_pd(v.e[2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[3]), _mm256_set1_pd(v.e[3])));

        vector result;
        _mm256_store_pd(result.e, a);

        return result;
    }

    vector matrix::transform_normalize_dir(const vector &v) const {
        // Multiply with first vector component
        __m256d a = _mm256_mul_pd(_mm256_load_pd(data[0]), _mm256_set1_pd(v.e[0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[1]), _mm256_set1_pd(v.e[1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[2]), _mm256_set1_pd(v.e[2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_load_pd(data[3]), _mm256_set1_pd(v.e[3])));
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

    vector matrix::transform_normal(const vector &n) const {
        // TODO CHECK THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // Multiply with first vector component
        __m256d a = _mm256_mul_pd(_mm256_set_pd(data[0][0], data[1][0], data[2][0], data[3][0]),
                                  _mm256_set1_pd(n.e[0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][1], data[1][1], data[2][1], data[3][1]),
                                           _mm256_set1_pd(n.e[1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][2], data[1][2], data[2][2], data[3][2]),
                                           _mm256_set1_pd(n.e[2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][3], data[1][3], data[2][3], data[3][3]),
                                           _mm256_set1_pd(n.e[3])));

        vector result;
        _mm256_store_pd(result.e, a);

        return result;
    }

    vector matrix::transform_normalize_normal(const vector &n) const {
        // TODO CHECK THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // Multiply with first vector component
        __m256d a = _mm256_mul_pd(_mm256_set_pd(data[0][0], data[1][0], data[2][0], data[3][0]),
                                  _mm256_set1_pd(n.e[0]));
        // Add second colum times second component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][1], data[1][1], data[2][1], data[3][1]),
                                           _mm256_set1_pd(n.e[1])));
        // Add third colum times third component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][2], data[1][2], data[2][2], data[3][2]),
                                           _mm256_set1_pd(n.e[2])));
        // Add fourth colum times fourth component
        a = _mm256_add_pd(a, _mm256_mul_pd(_mm256_set_pd(data[0][3], data[1][3], data[2][3], data[3][3]),
                                           _mm256_set1_pd(n.e[3])));

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

    void print_mat(const matrix &m) {
        std::cout << "[" << m.at(0, 0) << "\t" << m.at(0, 1) << "\t" << m.at(0, 2) << "\t" << m.at(0, 3) << "]"
                  << std::endl;
        std::cout << "[" << m.at(1, 0) << "\t" << m.at(1, 1) << "\t" << m.at(1, 2) << "\t" << m.at(1, 3) << "]"
                  << std::endl;
        std::cout << "[" << m.at(2, 0) << "\t" << m.at(2, 1) << "\t" << m.at(2, 2) << "\t" << m.at(2, 3) << "]"
                  << std::endl;
        std::cout << "[" << m.at(3, 0) << "\t" << m.at(3, 1) << "\t" << m.at(3, 2) << "\t" << m.at(3, 3) << "]"
                  << std::endl;
    }
}