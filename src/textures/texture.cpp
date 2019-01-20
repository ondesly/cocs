//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#define STB_RECT_PACK_IMPLEMENTATION

#include <stb/stb_rect_pack.h>
#include <pugixml/pugixml.hpp>

#include "core/engine.h"
#include "core/files.h"
#include "math/int2.h"
#include "textures/image_renderer.h"
#include "textures/text_renderer.h"

#include "texture.h"

std::shared_ptr<cc::texture> cc::texture::create(const std::vector<cc::texture::params> &frames) {
    return std::make_shared<cc::texture>(frames);
}

cc::float2 get_original_size(const char *s) {
    cc::float2 size;
    std::sscanf(s, "%*f %*f %f %f", &size.w, &size.h);
    return size;
}

cc::float2 get_frame_size(const cc::float2 &size, const cc::float2 &original_size) {
    if (size.w == 0) {
        return {original_size.w * size.h / original_size.h, size.h};
    } else if (size.h == 0) {
        return {size.w, original_size.h * size.w / original_size.w};
    } else {
        return size;
    }
}

cc::float2 get_actual_size(const cc::float2 &frame_size, const cc::float2 &original_size) {
    if (frame_size.w / original_size.w > frame_size.h / original_size.h) {
        return {frame_size.w, original_size.h * frame_size.w / original_size.w};
    } else if (frame_size.w / original_size.w < frame_size.h / original_size.h) {
        return {original_size.w * frame_size.h / original_size.h, frame_size.h};
    } else {
        return frame_size;
    }
}

cc::int2 get_texture_size(stbrp_rect *begin, stbrp_rect *end) {
    cc::int2 size = {0, 0};

    for (auto r = begin; r < end; ++r) {
        size.w = std::max(size.w, r->x + r->w);
        size.h = std::max(size.h, r->y + r->h);
    }

    return size;
}

pugi::xml_document *read_image(const std::string &path) {
    auto doc = new pugi::xml_document;

    auto content = cc::engine::i()->get_files()->get_content(path);
    auto result = doc->load_string(content.c_str());

    if (result.status == pugi::xml_parse_status::status_ok) {
        return doc;
    } else {
        delete doc;
        return nullptr;
    }
}

cc::texture::texture(const std::vector<cc::texture::params> &frames) {

    // Pack

    const int max_texture_size = 4096;

    stbrp_context ctx;
    auto nodes = new stbrp_node[max_texture_size];
    stbrp_init_target(&ctx, max_texture_size, max_texture_size, nodes, max_texture_size);

    auto src = new stbrp_rect[frames.size()];

    //

    std::vector<atlas_params> params;

    stbrp_rect *p_src = src;
    for (auto &frame : frames) {
        if (frame.is_font) {
            unsigned char *data;
            auto size = engine::i()->get_text_renderer()->render(frame.text, frame.size.h, frame.size.w, data);

            params.push_back({frame.name, nullptr, data, 0.f, {}});

            *p_src = {int(p_src - src), stbrp_coord(size.w), stbrp_coord(size.h), 0, 0, 0};
        } else {
            auto image = read_image(frame.path + "/" + frame.name + ".svg");

            if (!image) {
                return;
            }

            auto root = image->child("svg");
            auto original_size = get_original_size(root.attribute("viewBox").as_string());

            auto frame_size = get_frame_size(frame.size, original_size);
            auto actual_size = get_actual_size(frame_size, original_size);

            float scale = std::max(frame_size.w / original_size.w, frame_size.h / original_size.h);

            params.push_back({frame.name, image, nullptr, scale, (frame_size - actual_size) * frame.anchor});

            *p_src = {int(p_src - src), stbrp_coord(frame_size.w), stbrp_coord(frame_size.h), 0, 0, 0};
        }
        ++p_src;
    }

    //

    if (stbrp_pack_rects(&ctx, src, int(params.size())) == 0) {
        return;
    }

    //

    auto size = get_texture_size(src, src + params.size());

    auto data = new unsigned char[size.w * size.h * 4];
    std::fill(data, data + size.w * size.h * 4, '\0');

    image_renderer renderer(data, size.w, size.h);

    for (auto r = src; r < src + params.size(); ++r) {
        auto rect = float4(r->x, r->y, r->w, r->h);

        auto &p = params[r->id];

        if (p.doc) {
            renderer.render(p.doc->child("svg"), rect, p.scale, p.anchor);
        } else if (p.data) {
            int atlas_stride = size.w * 4;
            int atlas_begin = int(rect.x * 4 + rect.y * atlas_stride);
            int w = int(rect.w * 4);
            int h = int(rect.h);

            for (int i = 0; i < h; ++i) {
                std::copy(&p.data[i * w], &p.data[i * w + w], &data[atlas_begin + i * atlas_stride]);
            }
        }

        m_frames[p.name] = rect;
    }

    //

    set_data(data, size.w, size.h);

    //

    delete[] data;
    delete[] src;
    delete[] nodes;
}

const cc::float4 &cc::texture::get_frame(const std::string &name) {
    return m_frames[name];
}

void cc::texture::set_data(const unsigned char *data, const int width, const int height) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenTextures(1, &m_name);
    glBindTexture(GL_TEXTURE_2D, m_name);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    m_size = cc::float2(width, height);
}

cc::texture::~texture() {
    if (m_name) {
        glDeleteTextures(1, &m_name);
    }
}

GLuint cc::texture::get_name() const {
    return m_name;
}

const cc::float2 &cc::texture::get_size() const {
    return m_size;
}