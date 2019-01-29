//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "nodes/node.hpp"
#include "core/value.hpp"

#include "events.hpp"

void cc::events::dispatch_touches_began(const cc::value &points) {
    dispatch_touches(node::TOUCH_BEGAN, points);

    m_touches_count += points.size();
}

void cc::events::dispatch_touches_moved(const cc::value &points) {
    dispatch_touches(node::TOUCH_MOVED, points);
}

void cc::events::dispatch_touches_ended(const cc::value &points) {
    dispatch_touches(node::TOUCH_ENDED, points);

    m_touches_count -= points.size();

    if (m_touches_count == 0) {
        m_touch_target = nullptr;
    }
}

void cc::events::dispatch_touches_cancelled(const cc::value &points) {
    dispatch_touches(node::TOUCH_CANCELLED, points);

    m_touches_count -= points.size();

    if (m_touches_count == 0) {
        m_touch_target = nullptr;
    }
}

void cc::events::dispatch_touches(const int id, const cc::value &points) {
    if (m_touch_target) {
        if (m_touch_target->is_running()) {
            auto &listeners = m_listeners[id];
            auto it = listeners.find(m_touch_target);
            if (it != listeners.end()) {
                auto &fn = (*it).second;
                fn(points);
            }
        }
    } else {
        for (auto &pair : m_listeners[id]) {
            if (pair.first->is_running() && pair.first->is_contains(points.front())) {
                m_touch_target = pair.first;
                auto &fn = pair.second;
                fn(points);
                break;
            }
        }
    }
}

void cc::events::tick() {
    auto listeners = m_listeners[int(node::TICK)];
    for (auto &pair : listeners) {
        if (pair.first->is_running()) {
            pair.second({});
        }
    }
}

void cc::events::listen(cc::node *const target, const int id, const std::function<void(const cc::value &)> &fn) {
    m_listeners[id][target] = fn;
}

void cc::events::stop_listening(cc::node *const target, const int id/* = 0*/) {
    if (id == 0) {
        for (auto &pair : m_listeners) {
            pair.second.erase(target);
        }
    } else {
        m_listeners[id].erase(target);
    }

}