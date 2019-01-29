//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "core/events.hpp"

#include "node.hpp"

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
        m_anchor_in_size = {m_anchor.x * m_size.x, m_anchor.y * m_size.y};
        m_transform_updated = m_node_transform_dirty = true;
    }
}

const cc::float2 &cc::node::get_size() const {
    return m_size;
}

void cc::node::set_size(const cc::float2 &size) {
    if (m_size != size) {
        m_size = size;
        m_anchor_in_size = {m_anchor.x * m_size.x, m_anchor.y * m_size.y};
        m_transform_updated = m_node_transform_dirty = true;

        on_size_changed();
    }
}

void cc::node::set_parent(cc::node *parent) {
    m_parent = parent;
    m_transform_updated = m_node_transform_dirty = true;
}

void cc::node::add_child(const std::shared_ptr<cc::node> &child) {
    m_transform_updated = true;

    m_children.push_back(child);

    child->set_parent(this);

    if (m_is_running) {
        child->on_enter();
    }
}

void cc::node::remove_child(const std::shared_ptr<cc::node> &child) {
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

void cc::node::draw() {

}

void cc::node::update_transform(const mat4 &transform) {
    m_transform = transform;
    m_transform_updated = false;
}

void cc::node::visit(const mat4 &parent_transform, const bool is_parent_dirty) {
    if (!m_is_visible) {
        return;
    }

    // Transform

    bool is_dirty = is_parent_dirty || m_transform_updated;

    if (is_dirty) {
        update_transform(parent_transform * get_node_transform());
    }

    // Draw

    draw();

    // Children

    if (!m_children.empty()) {
        for (auto &child : m_children) {
            child->visit(m_transform, is_dirty);
        }
    }
}

void cc::node::resume() {
    for (const auto &child: m_children) {
        child->resume();
    }
}

void cc::node::pause() {
    for (const auto &child: m_children) {
        child->pause();
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

    stop_listening();

    for (const auto &child: m_children) {
        child->on_exit();
    }
}

void cc::node::on_size_changed() {

}

bool cc::node::is_running() const {
    return m_is_running;
}

bool cc::node::is_contains(const cc::float2 &point) {
    return true;
}

const cc::mat4 &cc::node::get_node_transform() {
    if (m_node_transform_dirty) {
        m_node_transform = cc::mat4::make_translation(m_position.x, -m_position.y);
        m_node_transform.rotate_z(-m_rotation);
        m_node_transform.scale(m_scale);
        m_node_transform.translate(-m_anchor_in_size.x, m_anchor_in_size.y);
    }

    m_node_transform_dirty = false;

    return m_node_transform;
}

const cc::color4 &cc::node::get_color() const {
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

void cc::node::listen(const int id, const std::function<void(const cc::value &)> &fn) {
    cc::events::i()->listen(this, id, fn);
}

void cc::node::stop_listening(const int id) {
    cc::events::i()->stop_listening(this, id);
}
