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
 * File:   box_film.h
 * Author: simon
 *
 * Created on October 23, 2016, 9:30 PM
 */

#ifndef BOX_FILM_H
#define BOX_FILM_H

#include "film.h"

namespace pixel {

    // Define box filter film
    class BoxFilterFilm : public Film {
    public:
        // Constructor
        BoxFilterFilm(uint32_t w, uint32_t h);

        // Destructor
        ~BoxFilterFilm();

        // Add sample to the film
        bool AddSample(const SSESpectrum &s, float x, float y) override;

        // Get film color at a given coordinate
        SSESpectrum GetSpectrum(uint32_t i, uint32_t j) const override;

    private:
        // Unnormalized samples
        SSESpectrum *raster;
        // Number of samples added per-pixel
        uint32_t *num_samples;
    };
}


#endif /* BOX_FILM_H */

