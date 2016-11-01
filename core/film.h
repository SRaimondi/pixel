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
 * File:   film.h
 * Author: simon
 *
 * Created on October 23, 2016, 8:49 PM
 */

#ifndef FILM_H
#define FILM_H

#include "sse_spectrum.h"

namespace pixel {

    // Define base film class
    class Film {
    public:
        // Constructor
        Film(uint32_t w, uint32_t h);

        // Destructor
        virtual ~Film();

        // Add sample to the film
        virtual bool AddSample(const SSESpectrum &s, float x, float y) = 0;

        // Get film color at a given coordinate
        virtual SSESpectrum GetSpectrum(uint32_t i, uint32_t j) const = 0;

        // Get width and height of the film
        uint32_t GetWidth() const;

        uint32_t GetHeight() const;

    protected:
        // Film dimension
        const uint32_t width, height;
    };

}

#endif /* FILM_H */

