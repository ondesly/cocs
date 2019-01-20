//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "node.h"

std::shared_ptr<cc::node> cc::node::create() {
    return std::make_shared<cc::node>();
}

cc::node::~node() {
    for (auto &child : m_children) {
        child->m_parent = nullptr;
    }
}

float cc::node::get_rotation() const {
    return m_rotation;
}

void cc::node::set_rotation(float rotation) {
    if (m_rotation != rotation) {
        m_transform_updated = m_node_transform_dirty = true;
        m_rotation = rotation;
    }
}

const cc::float2 &cc::node::get_scale() const {
    return m_scale;
}

void cc::node::set_scale(float scale) {
    set_scale({scale, scale});
}

void cc::node::set_scale(const cc::float2 &scale) {
    if (m_scale != scale) {
        m_transform_updated = m_node_transform_dirty = true;
        m_scale = scale;
    }
}

const cc::float2 &cc::node::get_position() const {
    return m_position;
}

void cc::node::set_position(const float2 &pos) {
    if (m_position != pos) {
        m_transform_updated = m_node_transform_dirty = true;
        m_position = pos;
    }
}

bool cc::node::is_visible() const {
    return m_is_visible;
}

void cc::node::set_visible(bool visible) {
    if (visible != m_is_visible) {
        m_is_visible = visible;

        if (m_is_visible) {
            m_transform_updated = m_node_transform_dirty = true;
        }
    }
}

const cc::float2 &cc::node::get_anchor() const {
    return m_anchor;
}

void cc::node::set_anchor(const float2 &anchor) {
    if (m_anchor != anchor) {
        m_anchor = anchor;
        m_anchor_in_size = {-m_anchor.x * m_size.x, -m_anchor.y * m_size.y};
        m_transform_updated = m_node_transform_dirty = true;
    }
}

const cc::float2 &cc::node::get_size() const {
    return m_size;
}

void cc::node::set_size(const cc::float2 &size) {
    if (m_size != size) {
        m_size = size;
        m_anchor_in_size = {-m_anchor.x * m_size.x, -m_anchor.y * m_size.y};
        m_transform_updated = m_node_transform_dirty = true;
    }
}

void cc::node::set_parent(cc::node *parent) {
    m_parent = parent;
    m_transform_updated = m_node_transform_dirty = true;
}

void cc::node::add_child(std::shared_ptr<cc::node> child) {
    m_transform_updated = true;

    m_children.push_back(child);

    child->set_parent(this);

    if (m_is_running) {
        child->on_enter();
    }
}

void cc::node::remove_child(std::shared_ptr<cc::node> child) {
    auto index = std::find(m_children.begin(), m_children.end(), child) - m_children.begin();
    if (index < m_children.size()) {
        if (m_is_running) {
            child->on_exit();
        }
        child->set_parent(nullptr);

        m_children.erase(m_children.begin() + index);
    }
}

void cc::node::remove_children() {
    for (const auto &child : m_children) {
        if (m_is_running) {
            child->on_exit();
        }

        child->set_parent(nullptr);
    }

    m_children.clear();
}

void cc::node::draw(cc::renderer *renderer) {

}

void cc::node::update_transform(cc::renderer *renderer, const mat4 &transform) {
    m_transform = transform;
    m_transform_updated = false;
}

void cc::node::visit(cc::renderer *const renderer, const mat4 &parent_transform, bool is_parent_dirty) {
    if (!m_is_visible) {
        return;
    }

    // Transform

    bool is_dirty = is_parent_dirty || m_transform_updated;

    if (is_dirty) {
        update_transform(renderer, parent_transform * get_node_transform());
    }

    // Draw

    draw(renderer);

    // Children

    if (!m_children.empty()) {
        for (auto &child : m_children) {
            child->visit(renderer, m_transform, is_dirty);
        }
    }
}

void cc::node::on_enter() {
    for (const auto &child: m_children) {
        child->on_enter();
    }

    m_is_running = true;
}

void cc::node::on_exit() {
    m_is_running = false;

    for (const auto &child: m_children) {
        child->on_exit();
    }
}

const cc::mat4 &cc::node::get_node_transform() {
    if (m_node_transform_dirty) {
        m_node_transform = cc::mat4::make_translation(m_position);
        m_node_transform.rotate_z(-m_rotation);
        m_node_transform.scale(m_scale);
        m_node_transform.translate(m_anchor_in_size);
    }

    m_node_transform_dirty = false;

    return m_node_transform;
}

const cc::color4 &cc::node::get_color(void) const {
    return m_color;
}

void cc::node::set_color(const color4 &color) {
    m_color = color;

    update_color();
}

std::vector<std::shared_ptr<cc::node>> &cc::node::get_children() {
    return m_children;
}

const std::vector<std::shared_ptr<cc::node>> &cc::node::get_children() const {
    return m_children;
}

cc::node *cc::node::get_parent() {
    return m_parent;
}

const cc::node *cc::node::get_parent() const {
    return m_parent;
}

void cc::node::update_color() {

}
