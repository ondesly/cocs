//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#define STB_RECT_PACK_IMPLEMENTATION

#include <stb/stb_rect_pack.h>

#include "math/int2.hpp"
#include "textures/frame.hpp"

#include "texture.hpp"

std::shared_ptr<cc::texture> cc::texture::create(const std::unordered_map<std::string, std::shared_ptr<cc::frame>> &frames, const bool has_mipmaps/* = false*/) {
    return std::make_shared<cc::texture>(frames, has_mipmaps);
}

int cc::texture::pow2roundup(int x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

cc::int2 cc::texture::get_texture_size(stbrp_rect *begin, stbrp_rect *end, const bool has_mipmaps) {
    cc::int2 size = {0, 0};

    for (auto r = begin; r < end; ++r) {
        size.w = std::max(size.w, r->x + r->w);
        size.h = std::max(size.h, r->y + r->h);
    }

    if (has_mipmaps) {
        size.w = pow2roundup(size.w);
        size.h = pow2roundup(size.h);
    }

    return size;
}

cc::texture::texture(const std::unordered_map<std::string, std::shared_ptr<cc::frame>> &frames, const bool has_mipmaps)
        : m_frames(frames), m_has_mipmaps(has_mipmaps) {
    load();
}

void cc::texture::load() {

    // Preparation

    stbrp_context ctx;
    auto nodes = new stbrp_node[MAX_TEXTURE_SIZE];
    stbrp_init_target(&ctx, MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, nodes, MAX_TEXTURE_SIZE);

    auto src = new stbrp_rect[m_frames.size()];

    // Frames load

    auto p_src = src;
    for (auto &pair : m_frames) {
        pair.second->load();
        pair.second->set_pack_rect(p_src++);
    }

    // Pack

    if (stbrp_pack_rects(&ctx, src, int(m_frames.size())) == 0) {
        return;
    }
    delete[] nodes;

    // Data

    auto tex_size = get_texture_size(src, p_src, m_has_mipmaps);

    auto data = new unsigned char[tex_size.w * tex_size.h * 4];
    std::fill(data, data + tex_size.w * tex_size.h * 4, '\0');

    // Render

    for (auto &pair : m_frames) {
        pair.second->update_rect();
        pair.second->render(data, tex_size);
        pair.second->clear();
    }

    // Set

    set_data(data, tex_size, m_has_mipmaps);

    // Clear

    delete[] data;
    delete[] src;
}

const cc::int4 &cc::texture::get_frame(const std::string &name) {
    return m_frames[name]->get_rect();
}

void cc::texture::set_data(const unsigned char *data, const cc::int2 &size, const bool has_mipmaps) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenTextures(1, &m_name);
    glBindTexture(GL_TEXTURE_2D, m_name);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (has_mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_size = cc::float2(size.w, size.h);
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

void cc::texture::check() {
    if (glIsTexture(m_name) != GL_TRUE) {
        load();
    }
}
