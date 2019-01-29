//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline color4::color4(const unsigned char c) : r(c), g(c), b(c), a(c) {

    }

    inline color4::color4(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
            : r(r), g(g), b(b), a(a) {

    }

    // -- Component accesses --

    inline unsigned char &color4::operator[](const size_t i) {
        switch (i) {
            default:
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
        }
    }

    inline const unsigned char &color4::operator[](const size_t i) const {
        switch (i) {
            default:
            case 0:
                return r;
            case 1:
                return g;
            case 2:
                return b;
            case 3:
                return a;
        }
    }

    inline color4 color4::make_alpha_premultiplied() {
        color4 c = *this;

        const auto p = c.a / 255.f;
        c.r *= p;
        c.g *= p;
        c.b *= p;

        return c;
    }

}