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
 * Created on November 12, 2016, 04:36 PM
 */

#ifndef CORE_TEXTURE_H_
#define CORE_TEXTURE_H_

#include "pixel.h"

namespace pixel {

    // Define TextureInterface base class
    template<typename T>
    class TextureInterface {
    public:
        // Destructor
        virtual ~TextureInterface() {}

        // Evaluate texture at given SurfaceInteraction
        virtual T Evaluate(const SurfaceInteraction &interaction) const = 0;
    };

    // Texture mapping base class
    class TextureMapping2DInterface {
    public:
        // Destructor
        virtual ~TextureMapping2DInterface() {}

        // Map UV coordinates
        virtual void Map(const SurfaceInteraction &interaction, float *u, float *v) const = 0;
    };

    // Uv texture mapping
    class UVMapping2D : public TextureMapping2DInterface {
    public:
        UVMapping2D(float su, float sv, float du, float dv);

        void Map(const SurfaceInteraction &interaction, float *u, float *v) const override;

    private:
        // Scaling
        const float su, sv;
        // Offset
        const float du, dv;
    };

}


#endif /* CORE_TEXTURE_H_ */
