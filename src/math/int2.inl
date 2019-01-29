//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline int2::int2(const int s) : x(s), y(s) {
    }

    inline int2::int2(const int x, const int y) : x(x), y(y) {
    }

    // -- Component accesses --

    inline int &int2::operator[](const size_t i) {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
        }
    }

    inline const int &int2::operator[](const size_t i) const {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
        }
    }

}