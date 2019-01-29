//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <stb/stb_rect_pack.h>

#include "core/device.hpp"
#include "math/int2.hpp"
#include "math/int4.hpp"

#include "text_frame.hpp"

std::shared_ptr<cc::text_frame> cc::text_frame::make(const std::string &text, const float font_size, const float max_width) {
    return std::make_shared<text_frame>(text, font_size, max_width);
}

cc::text_frame::text_frame(const std::string &text, const float font_size, const float max_width)
        : m_text(text), m_font_size(font_size), m_max_width(max_width) {
}

cc::text_frame::~text_frame() {
    delete[] m_data;
}

void cc::text_frame::load() {
    auto size = device::i()->render_text(m_text, m_font_size, m_max_width, m_data);
    m_rect.w = int(size.w);
    m_rect.h = int(size.h);
}

void cc::text_frame::render(unsigned char *const data, const cc::int2 &data_size) {
    int atlas_stride = data_size.w * 4;
    int atlas_begin = m_rect.x * 4 + m_rect.y * atlas_stride;
    int w = m_rect.w * 4;

    for (int i = 0; i < m_rect.h; ++i) {
        std::copy(&m_data[i * w], &m_data[i * w + w], &data[atlas_begin + i * atlas_stride]);
    }
}

void cc::text_frame::clear() {
    delete[] m_data;
    m_data = nullptr;
}
