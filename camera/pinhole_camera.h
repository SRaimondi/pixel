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
 * File:   pinhole_camera.h
 * Author: simon
 *
 * Created on October 26, 2016, 12:29 AM
 */

#ifndef PINHOLE_CAMERA_H
#define PINHOLE_CAMERA_H

#include "camera.h"
#include "sse_matrix.h"

namespace pixel {

    // Define pinhole camera class

    class PinholeCamera : public CameraInterface {
    public:
        // Constructor
        PinholeCamera(const SSEVector &eye, const SSEVector &at, const SSEVector &up,
                      float fov, uint32_t width, uint32_t height);

        // Create ray for a given couple of pixel coordinates and a sample
        Ray GenerateRay(uint32_t i, uint32_t j, float u1, float u2) const override;

    private:
        //Camera position
        SSEVector eye_world;
        // Transformation matrix
        SSEMatrix view_matrix;
        // Field of view
        float bottom, top, left, right;
        // Image size
        uint32_t width, height;
    };

}


#endif /* PINHOLE_CAMERA_H */

