//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct float4 {

        // -- Data --

        union {

            struct {

                float x, y, w, h;

            };

            struct {

                float left, top, right, bottom;

            };

        };

        // -- Implicit basic constructors --

        float4() = default;

        float4(const float4 &) = default;

        // -- Explicit basic constructors --

        explicit float4(float s);

        float4(float x, float y, float w, float h);

        // -- Component accesses --

        float &operator[](size_t i);

        const float &operator[](size_t i) const;

        // -- Unary arithmetic operators --

        float4 &operator+=(const float4 &f);

        float4 &operator*=(float s);

        float4 &operator*=(const float4 &f);

    };

    // -- Binary operators --

    float4 operator+(const float4 &f1, float4 const &f2);

    float4 operator*(const float4 &f, float s);

    float4 operator*(const float4 &f1, const float4 &f2);

}

#include "float4.inl"
