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

#include "sse_matrix.h"

namespace pixel {

    SSEMatrix Inverse(const SSEMatrix &m) {


        // Matrix inversion based on the Gauss method
        int indxc[4], indxr[4];
        int ipiv[4] = {0, 0, 0, 0};
        float minv[4][4];
        memcpy(minv, Transpose(m).data, 16 * sizeof(float));
        for (int i = 0; i < 4; i++) {
            int irow = 0, icol = 0;
            float big = 0.f;
            // Choose pivot
            for (int j = 0; j < 4; j++) {
                if (ipiv[j] != 1) {
                    for (int k = 0; k < 4; k++) {
                        if (ipiv[k] == 0) {
                            if (std::abs(minv[j][k]) >= big) {
                                big = std::abs(minv[j][k]);
                                irow = j;
                                icol = k;
                            }
                        } else if (ipiv[k] > 1) {
                            std::cerr << "Singular matrix given to inversion procedure" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows irow_ and icol_ for pivot
            if (irow != icol) {
                for (int k = 0; k < 4; ++k) {
                    std::swap(minv[irow][k], minv[icol][k]);
                }
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.f) {
                std::cerr << "Singular matrix given to inversion procedure" << std::endl;
                exit(EXIT_FAILURE);
            }

            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            float pivinv = 1.f / minv[icol][icol];
            minv[icol][icol] = 1.f;
            for (int j = 0; j < 4; j++) {
                minv[icol][j] *= pivinv;
            }

            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++) {
                if (j != icol) {
                    float save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++) {
                        minv[j][k] -= minv[icol][k] * save;
                    }
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = 3; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < 4; k++) {
                    std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
                }
            }
        }

        return SSEMatrix(minv[0][0], minv[0][1], minv[0][2], minv[0][3],
                         minv[1][0], minv[1][1], minv[1][2], minv[1][3],
                         minv[2][0], minv[2][1], minv[2][2], minv[2][3],
                         minv[3][0], minv[3][1], minv[3][2], minv[3][3]);
    }

    void PrintSSEMatrix(const SSEMatrix &m) {
        for (size_t i = 0; i < 4; i++) {
            std::cout << "[";
            for (size_t j = 0; j < 4; j++) {
                std::cout << m.data[j][i];
                if (j < 3) {
                    std::cout << "\t";
                }
            }
            std::cout << "]" << std::endl;
        }
        std::cout << std::endl;
    }

}