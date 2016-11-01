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
 * File:   rectangle.h
 * Author: simon
 *
 * Created on October 31, 2016, 7:01 PM
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "pixel.h"
#include "shape.h"

namespace pixel {

    // Define rectangle class
    class Rectangle : public ShapeInterface {
    public:
        // Constructor
        Rectangle(float x_w, float z_w);

        bool Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const override;

        bool IntersectP(const Ray &ray) const override;

        float Area() const override;

        SurfaceInteraction Sample(float u1, float u2) const override;

        BBox ShapeBounding() const override;

    private:
        // Rectangle size
        float half_x_width, half_z_width;
    };

}

#endif /* RECTANGLE_H */

