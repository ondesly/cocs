//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "renderer/renderer.hpp"
#include "textures/texture.hpp"

#include "sprite.hpp"

std::shared_ptr<cc::sprite> cc::sprite::create(const std::shared_ptr<cc::texture> &texture, const std::string &frame_name/* = ""*/) {
    return std::make_shared<sprite>(texture, frame_name);
}

cc::sprite::sprite(const std::shared_ptr<cc::texture> &texture, const std::string &frame_name)
        : m_renderer(cc::renderer::i()), m_texture(texture) {
    if (frame_name.empty()) {
        set_size(texture->get_size());
    } else {
        auto frame_rect = texture->get_frame(frame_name);
        auto tex_size = m_texture->get_size();
        set_size(float2(frame_rect.w, frame_rect.h));

        m_vertices.tl.tex = {frame_rect.x / tex_size.w, frame_rect.y / tex_size.h};
        m_vertices.bl.tex = {frame_rect.x / tex_size.w, (frame_rect.y + frame_rect.h) / tex_size.h};
        m_vertices.br.tex = {(frame_rect.x + frame_rect.w) / tex_size.w, (frame_rect.y + frame_rect.h) / tex_size.h};
        m_vertices.tr.tex = {(frame_rect.x + frame_rect.w) / tex_size.w, frame_rect.y / tex_size.h};
    }
}

void cc::sprite::draw() {
    m_renderer->add_sprite(this);
}

void cc::sprite::update_color() {
    m_vertices.tl.color = m_vertices.bl.color = m_vertices.br.color = m_vertices.tr.color = m_color.make_alpha_premultiplied();

    m_renderer->update_render_area(m_vertices);
}

void cc::sprite::update_transform(const cc::mat4 &transform) {
    node::update_transform(transform);

    m_renderer->update_render_area(m_vertices);

    m_vertices.tl.pos = transform * cc::float2(0.f, 0.f);
    m_vertices.bl.pos = transform * cc::float2(0.f, -get_size().y);
    m_vertices.br.pos = transform * cc::float2(get_size().x, -get_size().y);
    m_vertices.tr.pos = transform * cc::float2(get_size().x, 0.f);

    m_renderer->update_render_area(m_vertices);
}

std::shared_ptr<cc::texture> cc::sprite::get_texture() const {
    return m_texture;
}

const cc::quad &cc::sprite::get_vertices() const {
    return m_vertices;
}

void cc::sprite::resume() {
    node::resume();

    m_texture->check();
}

void cc::sprite::on_exit() {
    node::on_exit();

    m_renderer->update_render_area(m_vertices);
}
