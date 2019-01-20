//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

namespace cc {

    class entity;

    struct float2;

    class touches {
    public:

        void dispatch_touches_began(const std::vector<cc::float2> &touches);

        void dispatch_touches_moved(const std::vector<cc::float2> &touches);

        void dispatch_touches_ended(const std::vector<cc::float2> &touches);

        void dispatch_touches_cancelled(const std::vector<cc::float2> &touches);

        void add_began_listener(cc::entity *const target, const std::function<bool(const std::vector<cc::float2> &)> &func);

        void add_moved_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func);

        void add_ended_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func);

        void add_cancelled_listener(cc::entity *const target, const std::function<void(const std::vector<cc::float2> &)> &func);

        void remove_listener(cc::entity *const target);

    private:

        std::unordered_map<cc::entity *, std::function<bool(const std::vector<cc::float2> &)>> m_began_listeners;
        std::unordered_map<cc::entity *, std::function<void(const std::vector<cc::float2> &)>> m_moved_listeners;
        std::unordered_map<cc::entity *, std::function<void(const std::vector<cc::float2> &)>> m_ended_listeners;
        std::unordered_map<cc::entity *, std::function<void(const std::vector<cc::float2> &)>> m_cancelled_listeners;

    };

}