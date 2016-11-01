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
 * File:   transform.h
 * Author: simon
 *
 * Created on October 25, 2016, 11:09 PM
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "pixel.h"

namespace pixel {

    // Functions to generate a specific transformation matrix
    SSEMatrix Translate(float x, float y, float z);

    SSEMatrix Scale(float sx, float sy, float sz);

    SSEMatrix RotateX(float deg);

    SSEMatrix RotateY(float deg);

    SSEMatrix RotateZ(float deg);

    SSEMatrix LookAt(const SSEVector &eye, const SSEVector &at, const SSEVector &up);

}

#endif /* TRANSFORM_H */

