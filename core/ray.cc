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

#include "ray.h"
#include "sse_matrix.h"

namespace pixel {

    Ray::Ray(const SSEVector &o, const SSEVector &d, float tmin, float tmax, uint32_t depth)
            : o(o), d(d), tmin(tmin), tmax(tmax), depth(depth),
              inv_d(1.f / d.x, 1.f / d.y, 1.f / d.z, 0.f) {
    }

    void PrintRay(const Ray &r) {
        // Print origin
        std::cout << "Ray origin: ";
        print_sse_vector(r.Origin());
        // Print direction
        std::cout << "Direction: ";
        print_sse_vector(r.Direction());
        // Print ray maximum and minimum
        std::cout << "T min: " << r.RayMinimum() << std::endl;
        std::cout << "T max: " << r.RayMaximum() << std::endl;
        // Print ray depth
        std::cout << "Depth: " << r.RayDepth() << std::endl;
    }

    Ray TransformRay(const Ray &ray, const SSEMatrix &mat) {
        return Ray(mat * ray.Origin(), mat * ray.Direction(), ray.RayMinimum(), ray.RayMaximum(), ray.RayDepth());
    }

}