//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
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

        explicit int2(int s);

        int2(int x, int y);

        // -- Component accesses --

        int &operator[](size_t i);

        const int &operator[](size_t i) const;

    };

}

#include "int2.inl"