//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "core/engine.h"
#include "core/audio.h"
#include "core/scheduler.h"
#include "core/touches.h"
#include "math/float2.h"

#include "entity.h"

cc::entity::~entity() {
    refuse_to_listen_all();
}

bool cc::entity::is_running() const {
    return m_is_running;
}

void cc::entity::listen_tick(const std::function<void()> &func) {
    cc::engine::i()->get_scheduler()->schedule_update(this, func);
}

void cc::entity::refuse_to_listen_tick() {
    cc::engine::i()->get_scheduler()->unschedule_update(this);
}

void cc::entity::listen_touches_began(const std::function<bool(const std::vector<cc::float2> &)> &func) {
    cc::engine::i()->get_touches()->add_began_listener(this, func);
}

void cc::entity::listen_touches_moved(const std::function<void(const std::vector<cc::float2> &)> &func) {
    cc::engine::i()->get_touches()->add_moved_listener(this, func);
}

void cc::entity::listen_touches_ended(const std::function<void(const std::vector<cc::float2> &)> &func) {
    cc::engine::i()->get_touches()->add_ended_listener(this, func);
}

void cc::entity::listen_touches_cancelled(const std::function<void(const std::vector<cc::float2> &)> &func) {
    cc::engine::i()->get_touches()->add_cancelled_listener(this, func);
}

void cc::entity::refuse_to_listen_touches() {
    cc::engine::i()->get_touches()->remove_listener(this);
}

void cc::entity::refuse_to_listen_all() {
    refuse_to_listen_tick();
}

void cc::entity::preload(const std::vector<std::string> &paths) {
    cc::engine::i()->get_audio()->preload(paths);
}

void cc::entity::play(const std::string &path) {
    cc::engine::i()->get_audio()->play(path);
}

void cc::entity::stop(const std::string &path) {
    cc::engine::i()->get_audio()->stop(path);
}
