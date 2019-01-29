//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <cmath>
#include <pugixml/pugixml.hpp>
#include <stb/stb_rect_pack.h>

#include "core/device.hpp"
#include "math/float4.hpp"
#include "math/int2.hpp"
#include "textures/image_renderer.hpp"

#include "svg_frame.hpp"

std::shared_ptr<cc::svg_frame> cc::svg_frame::make(const std::string &path, const cc::float2 &size, const cc::float2 &anchor) {
    return std::make_shared<cc::svg_frame>(path, size, anchor);
}

cc::svg_frame::svg_frame(const std::string &path, const cc::float2 &size, const cc::float2 &anchor)
        : m_path(path), m_size(size), m_anchor(anchor) {
}

cc::float2 cc::svg_frame::get_original_size(const char *s) {
    cc::float2 size;
    std::sscanf(s, "%*f %*f %f %f", &size.w, &size.h);
    return size;
}

cc::float2 cc::svg_frame::get_frame_size(const cc::float2 &size, const cc::float2 &original_size) {
    if (size.w == 0) {
        return {std::ceil(original_size.w * size.h / original_size.h), size.h};
    } else if (size.h == 0) {
        return {size.w, std::ceil(original_size.h * size.w / original_size.w)};
    } else {
        return size;
    }
}

cc::float2 cc::svg_frame::get_actual_size(const cc::float2 &frame_size, const cc::float2 &original_size) {
    if (frame_size.w / original_size.w > frame_size.h / original_size.h) {
        return {frame_size.w, original_size.h * frame_size.w / original_size.w};
    } else if (frame_size.w / original_size.w < frame_size.h / original_size.h) {
        return {original_size.w * frame_size.h / original_size.h, frame_size.h};
    } else {
        return frame_size;
    }
}

pugi::xml_document *cc::svg_frame::read_image(const std::string &path) {
    auto doc = new pugi::xml_document;

    auto content = cc::device::i()->get_file_content(path);
    auto result = doc->load_string(content.c_str());

    if (result.status == pugi::xml_parse_status::status_ok) {
        return doc;
    } else {
        delete doc;
        return nullptr;
    }
}

void cc::svg_frame::load() {
    m_doc = read_image(m_path);

    if (!m_doc) {
        return;
    }

    auto root = m_doc->child("svg");
    auto original_size = get_original_size(root.attribute("viewBox").as_string());

    auto frame_size = get_frame_size(m_size, original_size);

    auto actual_size = get_actual_size(frame_size, original_size);
    m_anchor_in_size = (frame_size - actual_size) * m_anchor;

    m_scale = std::max(frame_size.w / original_size.w, frame_size.h / original_size.h);

    m_rect.w = int(frame_size.w);
    m_rect.h = int(frame_size.h);
}

void cc::svg_frame::render(unsigned char *const data, const cc::int2 &data_size) {
    cc::image_renderer renderer(data, data_size.w, data_size.h);

    renderer.render(m_doc->child("svg"), m_rect, m_scale, m_anchor_in_size);
}

cc::svg_frame::~svg_frame() {
    delete m_doc;
}

void cc::svg_frame::clear() {
    delete m_doc;
    m_doc = nullptr;
}
