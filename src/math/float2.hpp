//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct float2 {

        // -- Data --

        union {

            struct {

                float x, y;

            };

            struct {

                float w, h;

            };

        };

        // -- Implicit basic constructors --

        float2() = default;

        float2(const float2 &) = default;

        // -- Explicit basic constructors --

        explicit float2(float s);

        float2(float x, float y);

        // -- Component accesses --

        float &operator[](size_t i);

        const float &operator[](size_t i) const;

        // -- Unary arithmetic operators --

        float2 &operator+=(const float2 &f);

        float2 &operator-=(const float2 &f);

        float2 &operator*=(const float2 &f);

        float2 &operator/=(const float2 &f);

    };

    // -- Binary operators --

    bool operator==(const cc::float2 &f1, const cc::float2 &f2);

    bool operator!=(const cc::float2 &f1, const cc::float2 &f2);

    cc::float2 operator+(const cc::float2 &f1, const cc::float2 &f2);

    cc::float2 operator-(const cc::float2 &f1, const cc::float2 &f2);

    cc::float2 operator*(const cc::float2 &f1, const cc::float2 &f2);

    cc::float2 operator/(float s, const cc::float2 &f);

    cc::float2 operator/(const cc::float2 &f1, const cc::float2 &f2);

}

#include "float2.inl"
