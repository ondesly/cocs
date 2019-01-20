//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
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

        explicit int4(const int s);

        int4(const int x, const int y, const int w, const int h);

        // -- Component accesses --

        int &operator[](const size_t i);

        const int &operator[](const size_t i) const;


    };

}

#include "int4.inl"
