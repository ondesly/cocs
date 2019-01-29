//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <cstddef>

namespace cc {

    struct color4 {

        // -- Data --

        union {

            struct {

                unsigned char r, g, b, a;

            };

        };

        // -- Implicit basic constructors --

        color4() = default;

        color4(const color4 &) = default;

        // -- Explicit basic constructors --

        explicit color4(unsigned char c);

        color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        // -- Component accesses --

        unsigned char &operator[](size_t i);

        const unsigned char &operator[](size_t i) const;

        // --  --

        color4 make_alpha_premultiplied();

    };

}

#include "color4.inl"