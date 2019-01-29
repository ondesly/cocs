//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <algorithm>

#include <clipper/clipper.hpp>

#include "math/float2.hpp"
#include "renderer/vertex.hpp"

#include "render_area.hpp"

cc::render_area::render_area(const size_t render_buffers_count) : m_render_buffers_count(render_buffers_count) {

}

void cc::render_area::add(const cc::quad &vertices) {
    if (m_vertices_count == MAX_VERTICES_COUNT) {
        return;
    }

    m_xs[m_vertices_count] = vertices.tl.pos.x;
    m_ys[m_vertices_count++] = vertices.tl.pos.y;

    m_xs[m_vertices_count] = vertices.bl.pos.x;
    m_ys[m_vertices_count++] = vertices.bl.pos.y;

    m_xs[m_vertices_count] = vertices.br.pos.x;
    m_ys[m_vertices_count++] = vertices.br.pos.y;

    m_xs[m_vertices_count] = vertices.tr.pos.x;
    m_ys[m_vertices_count++] = vertices.tr.pos.y;
}

bool cc::render_area::is_changed() const {
    return m_frames_from_last_change < m_render_buffers_count + 2 || m_frames_to_skip != 0;
}

void cc::render_area::update() {
    if (m_vertices_count == 0) {
        ++m_frames_from_last_change;
        return;
    }

    m_frames_from_last_change = 0;

    if (m_vertices_count == MAX_VERTICES_COUNT) {
        return;
    }

    const auto x_it = std::minmax_element(m_xs, m_xs + m_vertices_count);
    const auto y_it = std::minmax_element(m_ys, m_ys + m_vertices_count);

    m_lefts.push_back(*x_it.first);
    m_tops.push_back(*y_it.second);
    m_rights.push_back(*x_it.second);
    m_bottoms.push_back(*y_it.first);

    if (m_lefts.size() > m_render_buffers_count) {
        m_lefts.pop_front();
        m_tops.pop_front();
        m_rights.pop_front();
        m_bottoms.pop_front();
    }

    m_area = {
            *std::min_element(m_lefts.begin(), m_lefts.end()),
            *std::max_element(m_tops.begin(), m_tops.end()),
            *std::max_element(m_rights.begin(), m_rights.end()),
            *std::min_element(m_bottoms.begin(), m_bottoms.end())
    };
}

void cc::render_area::done() {
    m_vertices_count = 0;
    m_frames_to_skip = m_frames_to_skip == 0 ? 0 : --m_frames_to_skip;
}

void cc::render_area::reset() {
    m_frames_to_skip = m_render_buffers_count;
}

cc::render_area::status cc::render_area::get_intersection_status(const cc::quad &vertices) const {
    if (m_vertices_count == MAX_VERTICES_COUNT || m_frames_to_skip != 0) {
        return status::INSIDE;
    }

    const float x_min = std::min({vertices.tl.pos.x, vertices.bl.pos.x, vertices.br.pos.x, vertices.tr.pos.x});
    const float x_max = std::max({vertices.tl.pos.x, vertices.bl.pos.x, vertices.br.pos.x, vertices.tr.pos.x});
    const float y_min = std::min({vertices.tl.pos.y, vertices.bl.pos.y, vertices.br.pos.y, vertices.tr.pos.y});
    const float y_max = std::max({vertices.tl.pos.y, vertices.bl.pos.y, vertices.br.pos.y, vertices.tr.pos.y});

    // Sprite is outside

    if (m_area.left > x_max || m_area.right < x_min || m_area.top < y_min || m_area.bottom > y_max) {
        return status::OUTSIDE;
    }

    // Sprite is inside

    if (m_area.left <= x_min && m_area.right >= x_max && m_area.top >= y_max && m_area.bottom <= y_min) {
        return status::INSIDE;
    }

    // Sprite is intersected

    return status::INTERSECTION;
}

std::vector<cc::float2> cc::render_area::get_intersection(const cc::quad &vertices) {
    static const float MULTIPLIER = 10000.f;

    ClipperLib::Clipper clipper;

    clipper.AddPaths({{
            {int64_t(m_area.left * MULTIPLIER), int64_t(m_area.top * MULTIPLIER)},
            {int64_t(m_area.left * MULTIPLIER), int64_t(m_area.bottom * MULTIPLIER)},
            {int64_t(m_area.right * MULTIPLIER), int64_t(m_area.bottom * MULTIPLIER)},
            {int64_t(m_area.right * MULTIPLIER), int64_t(m_area.top * MULTIPLIER)}
    }}, ClipperLib::ptSubject, true);

    clipper.AddPaths({{
            {int64_t(vertices.tl.pos.x * MULTIPLIER), int64_t(vertices.tl.pos.y * MULTIPLIER)},
            {int64_t(vertices.bl.pos.x * MULTIPLIER), int64_t(vertices.bl.pos.y * MULTIPLIER)},
            {int64_t(vertices.br.pos.x * MULTIPLIER), int64_t(vertices.br.pos.y * MULTIPLIER)},
            {int64_t(vertices.tr.pos.x * MULTIPLIER), int64_t(vertices.tr.pos.y * MULTIPLIER)}
    }}, ClipperLib::ptClip, true);

    ClipperLib::Paths solution;
    clipper.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

    //

    std::vector<cc::float2> result;

    if (!solution.empty()) {
        for (auto point : solution.front()) {
            result.emplace_back(point.X / MULTIPLIER, point.Y / MULTIPLIER);
        }
    }

    return result;
}
