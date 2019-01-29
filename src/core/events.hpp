//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <functional>
#include <unordered_map>

#include "core/singleton.hpp"

namespace cc {

    class value;

    class node;

    class events : public singleton<events> {
        friend class singleton<events>;

    public:

        void dispatch_touches_began(const cc::value &points);

        void dispatch_touches_moved(const cc::value &points);

        void dispatch_touches_ended(const cc::value &points);

        void dispatch_touches_cancelled(const cc::value &points);

        void tick();

        void listen(cc::node *target, int id, const std::function<void(const cc::value &)> &fn);

        void stop_listening(cc::node *target, int id = 0);

    private:

        std::unordered_map<int, std::unordered_map<cc::node *, std::function<void(const cc::value &)>>> m_listeners;

        cc::node *m_touch_target = nullptr;
        size_t m_touches_count = 0;

    private:

        void dispatch_touches(int id, const cc::value &points);

    };

}