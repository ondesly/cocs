//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline float4::float4(const float s) : x(s), y(s), w(s), h(s) {

    }

    inline float4::float4(const float x, const float y, const float w, const float h)
            : x(x), y(y), w(w), h(h) {

    }

    // -- Component accesses --

    inline float &float4::operator[](const size_t i) {
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

    inline const float &float4::operator[](const size_t i) const {
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

    // -- Unary arithmetic operators --

    inline float4 &float4::operator+=(const float4 &f) {
        return *this = float4(x + f.x, y + f.y, w + f.w, h + f.h);
    }

    inline float4 &float4::operator*=(const float s) {
        return *this = float4(x * s, y * s, w * s, h * s);
    }

    inline float4 &float4::operator*=(const float4 &f) {
        return *this = float4(x * f.x, y * f.y, w * f.w, h * f.h);
    }

    // -- Binary operators --

    inline float4 operator+(const float4 &f1, const float4 &f2) {
        return float4(f1) += f2;
    }

    inline float4 operator*(const float4 &f, const float s) {
        return float4(f) *= s;
    }

    inline float4 operator*(const float4 &f1, const float4 &f2) {
        return float4(f1) *= f2;
    }

}
