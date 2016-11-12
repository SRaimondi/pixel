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
 * Created on November 12, 2016, 05:29 PM
 */

#ifndef PIXEL_CHECKBOARD_TEXTURE_H
#define PIXEL_CHECKBOARD_TEXTURE_H

#include "pixel.h"

namespace pixel {

    // Define constant texture
    template<typename T>
    class CheckboardTexture : public TextureInterface<T> {
    public:
        CheckboardTexture(const std::shared_ptr<const TextureMapping2DInterface> mapping,
                          const std::shared_ptr<const TextureInterface<SSESpectrum>> &t1,
                          const std::shared_ptr<const TextureInterface<SSESpectrum>> &t2)
                : mapping(mapping), tex1(t1), tex2(t2) {}

        T Evaluate(const SurfaceInteraction &interaction) const override {
            // Map new uv coordinates
            float u, v;
            mapping->Map(interaction, &u, &v);
            if ((static_cast<uint32_t>(std::floor(u)) + static_cast<uint32_t>(std::floor(v))) % 2
                == 0) {
                return tex1->Evaluate(interaction);
            }
            return tex2->Evaluate(interaction);
        }

    private:
        // Mapping
        const std::shared_ptr<const TextureMapping2DInterface> mapping;
        // Colors
        const std::shared_ptr<const TextureInterface<SSESpectrum>> tex1, tex2;
    };

}

#endif //PIXEL_CHECKBOARD_TEXTURE_H
