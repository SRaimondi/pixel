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
 * File:   scene.h
 * Author: simon
 *
 * Created on October 27, 2016, 12:51 AM
 */

#ifndef SCENE_H
#define SCENE_H

#include "pixel.h"
#include "shape.h"

namespace pixel {

    // Define scene class
    class Scene {
    public:
        // Constructor
        Scene(const PrimitiveInterface *const root);

        // Add light to the scene
        void AddLight(const LightInterface *l);

        // Access the list of lights
        std::vector<const LightInterface *> const &GetLights() const;

        // Compute intersection of a ray with scene
        bool Intersect(const Ray &r, SurfaceInteraction *const interaction) const;

        // Check for intersection with scene
        bool IntersectP(const Ray &r) const;

    private:
        // Scene root primitive
        const PrimitiveInterface *const root;
        // List of lights in the scene
        std::vector<const LightInterface *> lights;
    };

}


#endif /* SCENE_H */

