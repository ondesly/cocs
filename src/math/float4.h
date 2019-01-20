//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
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

        explicit float4(const float s);

        float4(const float x, const float y, const float w, const float h);

        // -- Component accesses --

        float &operator[](const size_t i);

        const float &operator[](const size_t i) const;

        // -- Unary arithmetic operators --

        float4 &operator+=(const float4 &f);

        float4 &operator*=(const float s);

        float4 &operator*=(const float4 &f);

    };

    // -- Binary operators --

    float4 operator+(const float4 &f1, float4 const &f2);

    float4 operator*(const float4 &f, float s);

    float4 operator*(const float4 &f1, const float4 &f2);

}

#include "float4.inl"
