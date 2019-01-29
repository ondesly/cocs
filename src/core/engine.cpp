//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <iostream>
#include <utility>

#include "core/events.hpp"
#include "core/device.hpp"
#include "math/mat4.hpp"
#include "nodes/node.hpp"
#include "renderer/renderer.hpp"

#include "engine.hpp"

cc::engine::engine() : m_renderer(renderer::i()), m_events(events::i()), m_device(device::i()) {
    std::cout << "Cocs Micro Engine: v0.2" << std::endl;
}

cc::engine::~engine() {
    m_node->on_exit();
    m_node = nullptr;

    cc::renderer::destroy_instance();
    cc::events::destroy_instance();
    cc::device::destroy_instance();
}

void cc::engine::set_screen_size(const cc::float2 &size) {
    if (m_screen_size != size) {
        m_screen_size = size;
        m_is_screen_size_changed = true;
    }
}

const cc::float2 &cc::engine::get_screen_size() const {
    return m_screen_size;
}

void cc::engine::run(std::shared_ptr<cc::node> node) {
    m_node = std::move(node);
    m_node->on_enter();
}

bool cc::engine::is_running() const {
    return m_node != nullptr;
}

void cc::engine::tick() {
    if (m_paused) {
        return;
    }

    // Screen size

    if (m_is_screen_size_changed) {
        m_is_screen_size_changed = false;

        m_renderer->set_screen_size(m_screen_size);
        m_node->set_size(m_screen_size);
    }

    //

    m_events->tick();
    m_node->visit(cc::mat4(1.f), false);
    m_renderer->render();
}

void cc::engine::resume() {
    m_paused = false;

    m_device->play_music();
    m_node->resume();
    m_renderer->resume();
}

void cc::engine::pause() {
    m_paused = true;

    m_device->pause_music();
    m_node->pause();
}