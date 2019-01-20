//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <iostream>

#include "core/audio.h"
#include "core/files.h"
#include "core/scheduler.h"
#include "core/touches.h"
#include "math/mat4.h"
#include "nodes/node.h"
#include "renderer/renderer.h"
#include "textures/text_renderer.h"

#include "engine.h"

cc::engine *cc::engine::m_instance = nullptr;

cc::engine *cc::engine::i() {
    return m_instance;
}

cc::engine::engine(const cc::float2 &size, cc::files *const files, cc::text_renderer *const text_renderer, cc::audio *const audio) {
    m_instance = this;
    std::cout << "Engine initialized" << std::endl;

    m_screen_size = size;
    m_files = files;
    m_text_renderer = text_renderer;
    m_audio = audio;
    m_touches = new touches();
    m_scheduler = new scheduler();
    m_renderer = new renderer(m_screen_size);
}

cc::engine::~engine() {
    m_node->on_exit();
    m_node = nullptr;

    delete m_audio;
    delete m_scheduler;
    delete m_renderer;
    delete m_touches;
    delete m_files;
    delete m_text_renderer;
}

cc::touches *cc::engine::get_touches() const {
    return m_touches;
}

const cc::float2 &cc::engine::get_screen_size(void) const {
    return m_screen_size;
}

void cc::engine::run(std::shared_ptr<cc::node> node) {
    m_node = node;
    m_node->set_size(m_screen_size);
    m_node->on_enter();
}

void cc::engine::tick() {

    // Tick

    m_scheduler->tick();

    // Visit

    m_node->visit(m_renderer, cc::mat4(1.f), false);

    // Render

    m_renderer->render();
}

cc::files *cc::engine::get_files() {
    return m_files;
}

cc::text_renderer *cc::engine::get_text_renderer() {
    return m_text_renderer;
}

cc::audio *cc::engine::get_audio() {
    return m_audio;
}

cc::scheduler *cc::engine::get_scheduler() const {
    return m_scheduler;
}
