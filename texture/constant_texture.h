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
 * File:   texture.h
 * Author: simon
 *
 * Created on November 12, 2016, 04:43 PM
 */


#ifndef PIXEL_CONSTANTE_TEXTURE_H
#define PIXEL_CONSTANTE_TEXTURE_H

#include "pixel.h"

namespace pixel {

    // Define constant texture
    template<typename T>
    class ConstantTexture : public TextureInterface<T> {
    public:
        ConstantTexture(const T &v) : value(v) {}

        T Evaluate(const SurfaceInteraction &) const override {
            return value;
        }

    private:
        const T value;
    };

}

#endif //PIXEL_CONSTANTE_TEXTURE_H
