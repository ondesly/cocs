//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

#include "math/float2.h"
#include "math/float4.h"

namespace cc {

    struct mat4 {

        // -- Data --

        float4 value[4];

        // -- Transform makers --

        static mat4 make_translation(const cc::float2 &f);

        static mat4 make_scale(const cc::float2 &f);

        static mat4 make_ortho(const float left, const float right, const float bottom, const float top,
                const float znear, const float zfar);

        // -- Implicit basic constructors --

        mat4() = default;

        mat4(const mat4 &) = default;

        // -- Explicit basic constructors --

        explicit mat4(const float s);

        mat4(const float4 &f1, const float4 &f2, const float4 &f3, const float4 &f4);

        // -- Component accesses --

        float4 &operator[](const size_t i);

        const float4 &operator[](const size_t i) const;

        const float *value_ptr() const;

        // -- Unary arithmetic operators --

        mat4 &operator*=(const mat4 &m);

        // -- Transforms --

        void translate(const cc::float2 &f);

        void rotate_z(const float angle);

        void scale(const float2 &f);

    };

    // -- Binary operators --

    float4 operator*(mat4 const &m, float4 const &f);

    float2 operator*(mat4 const &m, float2 const &f);

    mat4 operator*(mat4 const &m1, mat4 const &m2);

}

#include "mat4.inl"