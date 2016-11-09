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
 * File:   pixel.h
 * Author: simon
 *
 * Created on October 21, 2016, 5:18 PM
 */

#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <cmath>
#include <string>
#include <complex>
#include <vector>

namespace pixel {

    // Forward declare project classes
    class Ray;

    class CameraInterface;

    class PinholeCamera;

    class Film;

    class BoxFilterFilm;

    class ToneMapperInterface;

    class ClampToneMapper;

    class SSEVector;

    class SSEMatrix;

    class SSESpectrum;

    class SurfaceInteraction;

    class ShapeInterface;

    class Sphere;

    class Rectangle;

    class PrimitiveInterface;

    class Instance;

    class PrimitiveList;

    // class ShapeList;

    class IntegratorInterface;

    class SurfaceIntegratorInterface;

    class DebugIntegrator;

    class DirectIntegrator;

    class WhittedIntegrator;

    class PathTracerIntegrator;

    class Scene;

    class RendererInterface;

    class SamplerRenderer;

    class BBox;

    class BRDF;

    class LambertianReflection;

    class SpecularReflection;

    class SpecularTransmission;

    // class FresnelSpecular;

    class BSDF;

    class MaterialInterface;

    class MatteMaterial;

    class EmittingMaterial;

    class MirrorMaterial;

    class GlassMaterial;

    class LightInterface;

    class OcclusionTester;

    class PointLight;

    class AreaLight;

    class FresnelInterface;

    class FresnelDielectric;

    class FresnelIdeal;

    // Declare constant values
    static float EPS = 10e-5f;
    static float PI = 3.14159265f;
    static float TWO_PI = 6.28318530718f;
    static float ONE_OVER_PI = 0.318309886184f;
    static float ONE_OVER_2_PI = 0.159154943092f;
    static float ONE_OVER_4_PI = 0.07957747154f;

    // Maximum and minimum functions
    template<typename T>
    inline T FMin(const T a, const T b) {
        return (a < b ? a : b);
    }

    template<typename T>
    inline T FMax(const T a, const T b) {
        return (a > b ? a : b);
    }

    // Clamp value
    template<typename T>
    inline T Clamp(const T val, const T min, const T max) {
        return FMin(FMax(val, min), max);
    }

    // Degree to radians
    template<typename T>
    inline T DegToRad(const T deg) {
        return ((PI / 180.f) * deg);
    }

    // Radians to degree
    template<typename T>
    inline T RadToDeg(const T rad) {
        return ((180.f / PI) * rad);
    }
}

#endif /* PIXEL_H */

