//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

namespace cc {

    // -- Explicit basic constructors --

    inline float2::float2(const float s) : x(s), y(s) {
    }

    inline float2::float2(const float x, const float y) : x(x), y(y) {
    }

    // -- Component accesses --

    inline float &float2::operator[](const size_t i) {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
        }
    }

    inline const float &float2::operator[](const size_t i) const {
        switch (i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
        }
    }

    // -- Unary arithmetic operators --

    inline float2 &float2::operator+=(const float2 &f) {
        return *this = float2(x + f.x, y + f.y);
    }

    inline float2 &float2::operator-=(const float2 &f) {
        return *this = float2(x - f.x, y - f.y);
    }

    inline float2 &float2::operator*=(const float2 &f) {
        return *this = float2(x * f.x, y * f.y);
    }

    inline float2 &float2::operator/=(const float2 &f) {
        return *this = float2(x / f.x, y / f.y);
    }

    // -- Binary operators --

    inline bool operator==(const float2 &f1, const float2 &f2) {
        return f1.x == f2.x && f1.y == f2.y;
    }

    inline bool operator!=(const float2 &f1, const float2 &f2) {
        return !(f1 == f2);
    }

    inline float2 operator+(const float2 &f1, const float2 &f2) {
        return float2(f1) += f2;
    }

    inline float2 operator-(const float2 &f1, const float2 &f2) {
        return float2(f1) -= f2;
    }

    inline float2 operator*(const float2 &f1, const float2 &f2) {
        return float2(f1) *= f2;
    }

    inline float2 operator/(const float s, const float2 &f) {
        return {s / f.x, s / f.y};
    }

    inline float2 operator/(const float2 &f1, const float2 &f2) {
        return float2(f1) /= f2;
    }

}