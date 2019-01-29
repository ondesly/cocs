//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "math/mat4.hpp"
#include "nodes/sprite.hpp"
#include "renderer/render_area.hpp"
#include "renderer/program.hpp"
#include "textures/texture.hpp"

#include "renderer.hpp"

cc::renderer::renderer() {

    // Blend

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Buffers

    glGenBuffers(1, &m_array_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_array_buffer);

    glGenBuffers(1, &m_element_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_array_buffer);

    //

    m_program = new program();
    m_render_area = new cc::render_area(RENDER_BUFFERS_COUNT);
}

void cc::renderer::set_screen_size(const cc::float2 &size) {
    glViewport(0, 0, GLsizei(size.w), GLsizei(size.h));

    const auto projection_mat = mat4::make_ortho(0.f, size.w, -size.h, 0, 1.f, -1.f);
    m_program->set_projection_mat(projection_mat);

    m_render_area->reset();
}

cc::renderer::~renderer() {
    glDeleteBuffers(1, &m_array_buffer);
    glDeleteBuffers(1, &m_element_array_buffer);

    //

    delete m_program;
    delete m_render_area;
}

void cc::renderer::add_sprite(cc::sprite *const sprite) {
    m_queue.push_back(sprite);
}

void cc::renderer::update_render_area(const cc::quad &vertices) {
    m_render_area->add(vertices);
}

void cc::renderer::resume() {
    m_render_area->reset();
}

void cc::renderer::render() {
    m_render_area->update();

    if (!m_queue.empty() && m_render_area->is_changed()) {
        size_t filled_vertices_count = 0;
        size_t filled_indices_count = 0;

        for (const auto &sprite : m_queue) {

            // Fill vertices and indices

            auto vertices_count = fill_vertices(&m_vertices_buffer[filled_vertices_count], sprite);
            auto indices_count = fill_indices(&m_indices_buffer[filled_indices_count], filled_vertices_count, vertices_count);

            if (vertices_count == 0 || indices_count == 0) {
                continue;
            }

            filled_vertices_count += vertices_count;
            filled_indices_count += indices_count;

            // New batch

            if (m_batches.empty() || m_batches.back().texture_name != sprite->get_texture()->get_name()) {
                m_batches.push_back({sprite->get_texture()->get_name(), indices_count,
                        m_batches.empty() ? 0 : m_batches.back().indices_count + m_batches.back().offset});
            } else {
                m_batches.back().indices_count += indices_count;
            }
        }

        // Bind buffers

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * filled_vertices_count, m_vertices_buffer, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * filled_indices_count, m_indices_buffer, GL_DYNAMIC_DRAW);

        // Draw

        for (auto &batch : m_batches) {
            glBindTexture(GL_TEXTURE_2D, batch.texture_name);
            glDrawElements(GL_TRIANGLES, GLsizei(batch.indices_count), GL_UNSIGNED_SHORT, (GLvoid *) (batch.offset * sizeof(GLushort)));
        }
    }

    m_batches.clear();
    m_queue.clear();
    m_render_area->done();
}

size_t cc::renderer::fill_indices(GLushort *buffer, const size_t start, const size_t vertices_count) {
    static GLushort indices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4};

    size_t count = (vertices_count % 3 + 1) * 3;

    for (int i = 0; i < count; i++) {
        buffer[i] = GLushort(start) + indices[i];
    }

    return count;
}

size_t cc::renderer::fill_vertices(cc::vertex *buffer, cc::sprite *const sprite) {
    auto status = m_render_area->get_intersection_status(sprite->get_vertices());

    switch (status) {
        case cc::render_area::status::OUTSIDE:
            return 0;
        case cc::render_area::status::INSIDE:
            return std::copy(&sprite->get_vertices().tl, &sprite->get_vertices().tr + 1, buffer) - buffer;
        case cc::render_area::status::INTERSECTION:
            auto intersection = m_render_area->get_intersection(sprite->get_vertices());
            if (intersection.empty()) {
                return 0;
            } else {
                return fill_vertices_intersection(buffer, sprite, intersection);
            }
    }
}

size_t cc::renderer::fill_vertices_intersection(cc::vertex *buffer, cc::sprite *const sprite, const std::vector<cc::float2> &intersection) {
    auto mat = mat4::make_scale(1.f / sprite->get_scale() / sprite->get_texture()->get_size());
    mat.rotate_z(-sprite->get_rotation());

    auto &tl = sprite->get_vertices().tl;
    for (auto &pos : intersection) {
        buffer->pos = pos;
        buffer->color = tl.color;
        buffer->tex = tl.tex + mat * float2(pos.x - tl.pos.x, tl.pos.y - pos.y);

        ++buffer;
    }

    //

    return intersection.size();
}
