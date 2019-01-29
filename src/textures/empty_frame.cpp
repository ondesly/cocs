//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <stb/stb_rect_pack.h>

#include "math/int2.hpp"
#include "math/int4.hpp"

#include "empty_frame.hpp"

std::shared_ptr<cc::empty_frame> cc::empty_frame::make(const cc::float2 &size) {
    return std::make_shared<cc::empty_frame>(size);
}

cc::empty_frame::empty_frame(const cc::float2 &size) : m_size(size) {

}

void cc::empty_frame::load() {
    m_rect.w = int(m_size.w);
    m_rect.h = int(m_size.h);
}

void cc::empty_frame::render(unsigned char *const data, const cc::int2 &data_size) {
    int atlas_stride = data_size.w * 4;
    int atlas_begin = m_rect.x * 4 + m_rect.y * atlas_stride;
    int w = m_rect.w * 4;

    for (int i = 0; i < m_rect.h; ++i) {
        std::fill(&data[atlas_begin + i * atlas_stride], &data[atlas_begin + i * atlas_stride + w], 255);
    }
}

void cc::empty_frame::clear() {

}
