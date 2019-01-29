//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct int4 {

        // -- Data --

        union {

            struct {

                int x, y, w, h;

            };

            struct {

                int left, top, right, bottom;

            };

        };

        // -- Implicit basic constructors --

        int4() = default;

        int4(const int4 &) = default;

        // -- Explicit basic constructors --

        explicit int4(int s);

        int4(int x, int y, int w, int h);

        // -- Component accesses --

        int &operator[](size_t i);

        const int &operator[](size_t i) const;


    };

}

#include "int4.inl"
