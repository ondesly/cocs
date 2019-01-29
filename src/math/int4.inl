//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline int4::int4(const int s) : x(s), y(s), w(s), h(s) {

    }

    inline int4::int4(const int x, const int y, const int w, const int h)
            : x(x), y(y), w(w), h(h) {

    }

    // -- Component accesses --

    inline int &int4::operator[](const size_t i) {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return w;
            case 3:
                return h;
        }
    }

    inline const int &int4::operator[](const size_t i) const {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return w;
            case 3:
                return h;
        }
    }

}
