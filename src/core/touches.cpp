//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "core/entity.h"
#include "math/float2.h"

#include "touches.h"

void cc::touches::dispatch_touches_began(const std::vector<cc::float2> &touches) {
    for (auto &pair : m_began_listeners) {
        if (pair.first->is_running() && pair.second(touches)) {
            break;
        }
    }
}

void cc::touches::dispatch_touches_moved(const std::vector<cc::float2> &touches) {
    for (auto &pair : m_moved_listeners) {
        if (pair.first->is_running()) {
            pair.second(touches);
        }
    }
}

void cc::touches::dispatch_touches_ended(const std::vector<cc::float2> &touches) {
    for (auto &pair : m_ended_listeners) {
        if (pair.first->is_running()) {
            pair.second(touches);
        }
    }
}

void cc::touches::dispatch_touches_cancelled(const std::vector<cc::float2> &touches) {
    for (auto &pair : m_cancelled_listeners) {
        if (pair.first->is_running()) {
            pair.second(touches);
        }
    }
}

void cc::touches::add_began_listener(cc::entity *const target, const std::function<bool(const std::vector<cc::float2> &)> &func) {
    m_began_listeners[target] = func;
}

void cc::touches::add_moved_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func) {
    m_moved_listeners[target] = func;
}

void cc::touches::add_ended_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func) {
    m_ended_listeners[target] = func;
}

void cc::touches::add_cancelled_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func) {
    m_cancelled_listeners[target] = func;
}

void cc::touches::remove_listener(entity *const target) {
    m_began_listeners.erase(target);
    m_moved_listeners.erase(target);
    m_ended_listeners.erase(target);
    m_cancelled_listeners.erase(target);
}
