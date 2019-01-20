//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "renderer/renderer.h"
#include "textures/texture.h"

#include "sprite.h"

std::shared_ptr<cc::sprite> cc::sprite::create(std::shared_ptr<cc::texture> texture, const std::string &frame_name/* = ""*/) {
    return std::make_shared<sprite>(texture, frame_name);
}

cc::sprite::sprite(std::shared_ptr<cc::texture> texture, const std::string &frame_name)
        : m_texture(texture) {
    if (frame_name.empty()) {
        set_size(texture->get_size());
    } else {
        auto frame_rect = texture->get_frame(frame_name);
        auto tex_size = m_texture->get_size();
        set_size({frame_rect.w, frame_rect.h});

        m_vertices.tl.tex = {frame_rect.x / tex_size.w, frame_rect.y / tex_size.h};
        m_vertices.bl.tex = {frame_rect.x / tex_size.w, (frame_rect.y + frame_rect.h) / tex_size.h};
        m_vertices.br.tex = {(frame_rect.x + frame_rect.w) / tex_size.w, (frame_rect.y + frame_rect.h) / tex_size.h};
        m_vertices.tr.tex = {(frame_rect.x + frame_rect.w) / tex_size.w, frame_rect.y / tex_size.h};
    }
}

void cc::sprite::draw(renderer *renderer) {
    renderer->add_sprite(this);
}

void cc::sprite::update_color() {
    m_vertices.tl.color = m_vertices.bl.color = m_vertices.br.color = m_vertices.tr.color = m_color.make_alpha_premultiplied();
}

void cc::sprite::update_transform(cc::renderer *const renderer, const cc::mat4 &transform) {
    node::update_transform(renderer, transform);

    renderer->update_render_area(m_vertices);

    m_vertices.tl.pos = transform * cc::float2(0.f, get_size().y);
    m_vertices.bl.pos = transform * cc::float2(0.f, 0.f);
    m_vertices.br.pos = transform * cc::float2(get_size().x, 0.f);
    m_vertices.tr.pos = transform * cc::float2(get_size().x, get_size().y);

    renderer->update_render_area(m_vertices);
}

std::shared_ptr<cc::texture> cc::sprite::get_texture() const {
    return m_texture;
}

const cc::quad &cc::sprite::get_vertices() const {
    return m_vertices;
}