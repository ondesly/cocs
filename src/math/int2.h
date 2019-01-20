//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct int2 {

        // -- Data --

        union {

            struct {

                int x, y;

            };

            struct {

                int w, h;

            };

        };

        // -- Implicit basic constructors --

        int2() = default;

        int2(const int2 &) = default;

        // -- Explicit basic constructors --

        explicit int2(const int s);

        int2(const int x, const int y);

        // -- Component accesses --

        int &operator[](const size_t i);

        const int &operator[](const size_t i) const;

    };

}

#include "int2.inl"