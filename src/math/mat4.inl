//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <cmath>

namespace cc {

    // -- Transform makers --

    inline mat4 mat4::make_translation(const float x, const float y) {
        return {{1.f, 0.f, 0.f, 0.f},
                {0.f, 1.f, 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {x, y, 0.f, 1.f}};
    }

    inline mat4 mat4::make_scale(const float2 &f) {
        return {{f[0], 0.f, 0.f, 0.f},
                {0.f, f[1], 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f}};
    }

    inline mat4 mat4::make_ortho(const float left, const float right, const float bottom, const float top,
            const float znear, const float zfar) {
        const float delta_x = right - left;
        const float delta_y = top - bottom;
        const float delta_z = zfar - znear;

        return {{/* */2.f / delta_x,/*              */0.f,/*                      */0.f,/*                      */0.f},
                {/* */0.f,/*                        */2.0f / delta_y,/*           */0.f,/*                      */0.f},
                {/* */0.f,/*                        */0.f,/*                     */-2.f / delta_z,/*            */0.f},
                {/**/-(right + left) / delta_x,/* */-(top + bottom) / delta_y,/* */-(zfar + znear) / delta_z,/* */1.f}};
    }

    // -- Explicit basic constructors --

    inline mat4::mat4(const float s)
            : value{{s, 0.f, 0.f, 0.f}, {0.f, s, 0.f, 0.f}, {0.f, 0.f, s, 0.f}, {0.f, 0.f, 0.f, s}} {

    }

    inline mat4::mat4(const float4 &f1, const float4 &f2, const float4 &f3, const float4 &f4)
            : value{f1, f2, f3, f4} {

    }

    // -- Component accesses --

    inline float4 &mat4::operator[](const size_t i) {
        return value[i];
    }

    inline const float4 &mat4::operator[](const size_t i) const {
        return value[i];
    }

    inline const float *mat4::value_ptr() const {
        return &value[0].x;
    }

    // -- Unary arithmetic operators --

    inline mat4 &mat4::operator*=(const mat4 &m) {
        return *this = *this * m;
    }

    // -- Transforms --

    inline void mat4::translate(const float x, const float y) {
        value[3] = value[0] * x + value[1] * y + value[3];
    }

    inline void mat4::rotate_z(const float angle) {
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        *this = {value[0] * c + value[1] * s,
                value[0] * -s + value[1] * c,
                value[0] * 0.f + value[1] * 0.f + value[2] * 1.f,
                value[3]};
    }

    inline void mat4::scale(const float2 &f) {
        value[0] = value[0] * f[0];
        value[1] = value[1] * f[1];
    }

    // -- Binary operators --

    inline float4 operator*(const mat4 &m, const float4 &f) {
        const auto mov0(f[0]);
        const auto mov1(f[1]);
        const auto mul0 = m[0] * mov0;
        const auto mul1 = m[1] * mov1;
        const auto add0 = mul0 + mul1;
        const auto mov2(f[2]);
        const auto mov3(f[3]);
        const auto mul2 = m[2] * mov2;
        const auto mul3 = m[3] * mov3;
        const auto add1 = mul2 + mul3;
        const auto add2 = add0 + add1;
        return add2;
    }

    inline float2 operator*(const mat4 &m, const float2 &f) {
        const auto mov0(f[0]);
        const auto mov1(f[1]);
        const auto mul0 = m[0] * mov0;
        const auto mul1 = m[1] * mov1;
        const auto add0 = mul0 + mul1;
        const auto mul3 = m[3];
        const auto add2 = add0 + mul3;
        return {add2[0], add2[1]};
    }

    inline mat4 operator*(const mat4 &m1, const mat4 &m2) {
        const auto src_a0 = m1[0];
        const auto src_a1 = m1[1];
        const auto src_a2 = m1[2];
        const auto src_a3 = m1[3];

        const auto src_b0 = m2[0];
        const auto src_b1 = m2[1];
        const auto src_b2 = m2[2];
        const auto src_b3 = m2[3];

        return {
                src_a0 * src_b0[0] + src_a1 * src_b0[1] + src_a2 * src_b0[2] + src_a3 * src_b0[3],
                src_a0 * src_b1[0] + src_a1 * src_b1[1] + src_a2 * src_b1[2] + src_a3 * src_b1[3],
                src_a0 * src_b2[0] + src_a1 * src_b2[1] + src_a2 * src_b2[2] + src_a3 * src_b2[3],
                src_a0 * src_b3[0] + src_a1 * src_b3[1] + src_a2 * src_b3[2] + src_a3 * src_b3[3]
        };
    }

}
