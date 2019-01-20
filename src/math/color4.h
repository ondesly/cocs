//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
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

        explicit color4(const unsigned char c);

        color4(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

        // -- Component accesses --

        unsigned char &operator[](const size_t i);

        const unsigned char &operator[](const size_t i) const;

        // --  --

        color4 make_alpha_premultiplied();

    };

}

#include "color4.inl"