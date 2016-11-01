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
 * File:   shape_list.h
 * Author: simon
 *
 * Created on October 30, 2016, 1:40 AM
 */

#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include "pixel.h"
#include "shape.h"

namespace pixel {

    // Define Shape List class

    class ShapeList : public ShapeInterface {
    public:
        // Constructor
        ShapeList();

        ShapeList(const std::vector<const ShapeInterface *> s);

        // Add Shape
        void AddShape(const ShapeInterface *s);

        bool Intersect(const Ray &ray, float *const t_hit, SurfaceInteraction *const interaction) const override;

        bool IntersectP(const Ray &ray) const override;

        BBox ShapeBounding() const override;

    private:
        // List of shapes
        std::vector<const ShapeInterface *> shapes;
    };

}

#endif /* SHAPE_LIST_H */

