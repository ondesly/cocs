//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <stb/stb_rect_pack.h>

#include "frame.hpp"

void cc::frame::set_pack_rect(stbrp_rect *const rect) {
    m_pack_rect = rect;
    m_pack_rect->w = stbrp_coord(m_rect.w + PADDING * 2);
    m_pack_rect->h = stbrp_coord(m_rect.h + PADDING * 2);
}

void cc::frame::update_rect() {
    m_rect.x = m_pack_rect->x + PADDING;
    m_rect.y = m_pack_rect->y + PADDING;
}

const cc::int4 &cc::frame::get_rect() {
    return m_rect;
}
