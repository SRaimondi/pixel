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
 * File:   bbox.h
 * Author: simon
 *
 * Created on October 31, 2016, 6:00 PM
 */

#ifndef BBOX_H
#define BBOX_H

#include "pixel.h"
#include "sse_vector.h"
#include "ray.h"

namespace pixel {

    // Define Bbox class

    class BBox {
    public:
        // Constructor
        BBox();

        BBox(const SSEVector &p);

        BBox(const SSEVector &p1, const SSEVector &p2);

        // Compute intersection of ray with BBox
        inline bool IntersectP(const Ray &ray) const {
            // Compute sign of direction
            int dir_is_neg[3] = { ray.Direction().x < 0.f, ray.Direction().y < 0.f, ray.Direction().z < 0.f};
            // Check intersection against x and y
            float t_min = (bounds[dir_is_neg[0]].x - ray.Origin().x) * ray.InvDirection().x;
            float t_max = (bounds[1 - dir_is_neg[0]].x - ray.Origin().x) * ray.InvDirection().x;
            float ty_min = (bounds[dir_is_neg[1]].y - ray.Origin().y) * ray.InvDirection().y;
            float ty_max = (bounds[1 - dir_is_neg[1]].y - ray.Origin().y) * ray.InvDirection().y;

            if (t_min > ty_max || ty_min > t_max) { return false; }
            if (ty_min > t_min) { t_min = ty_min; }
            if (ty_max < t_max) { t_max = ty_max; }

            // Check against against z
            float tz_min = (bounds[dir_is_neg[2]].z - ray.Origin().z) * ray.InvDirection().z;
            float tz_max = (bounds[1 - dir_is_neg[2]].z - ray.Origin().z) * ray.InvDirection().z;

            if (t_min > tz_max || tz_min > t_max) { return false; }
            if (tz_min > t_min) { t_min = tz_min; }
            if (tz_max < t_max) { t_max = tz_max; }

            return ((t_min < ray.RayMaximum()) && t_max > 0.f);
        }

        // Access minimum and maximum

        inline const SSEVector &Min() const {
            return bounds[0];
        }

        inline const SSEVector &Max() const {
            return bounds[1];
        }

        // Access bounding points by index
        inline const SSEVector &operator[](uint32_t i) const {
            return (i == 0) ? bounds[0] : bounds[1];
        }

        inline SSEVector &operator[](uint32_t i) {
            return (i == 0) ? bounds[0] : bounds[1];
        }

    private:
        // Maximum and minimum
        SSEVector bounds[2];
    };

    // Compute the union between two BBox
    BBox BBoxUnion(const BBox &b1, const BBox &b2);

    // Compute the union between a BBox and a point
    BBox BBoxUnion(const BBox &b, const SSEVector &p);

}

#endif /* BBOX_H */

