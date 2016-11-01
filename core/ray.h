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
 * File:   ray.h
 * Author: simon
 *
 * Created on October 22, 2016, 8:21 PM
 */

#ifndef RAY_H
#define RAY_H

#include "pixel.h"
#include "sse_vector.h"

namespace pixel {

    // Ray class
    class Ray {
    public:

        // Constructor
        Ray(const SSEVector &o, const SSEVector &d,
            float tmin = EPS, float tmax = INFINITY,
            uint32_t depth = 0);

        // Get ray origin
        inline const SSEVector &Origin() const {
            return o;
        }

        inline SSEVector &Origin() {
            return o;
        }

        // Get ray direction
        inline const SSEVector &Direction() const {
            return d;
        }

        inline SSEVector &Direction() {
            return d;
        }

        // Get ray inverse direction
        inline const SSEVector &InvDirection() const {
            return inv_d;
        }

        inline SSEVector &InvDirection() {
            return inv_d;
        }

        // Get ray maximum and minimum
        inline float RayMinimum() const {
            return tmin;
        }

        inline float RayMaximum() const {
            return tmax;
        }

        inline void SetNewMaximum(float new_max) const {
            tmax = new_max;
        }

        // Get ray depth
        inline uint32_t RayDepth() const {
            return depth;
        }

        // Find point at a given parameter
        inline SSEVector operator()(float t) const {
            return (o + t * d);
        }

    private:
        // Ray origin
        SSEVector o;
        // Ray direction
        SSEVector d;
        // Ray maximum and minimum
        float tmin;
        mutable float tmax;
        // Ray depth
        uint32_t depth;
        // Ray inverse direction
        SSEVector inv_d;
    };

    // Print ray to std::cout

    void PrintRay(const Ray &r);

    // Transform ray for a given matrix
    Ray TransformRay(const Ray &ray, const SSEMatrix &mat);

}

#endif /* RAY_H */

