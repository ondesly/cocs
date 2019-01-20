//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <functional>
#include <vector>

namespace cc {

    struct float2;

    class entity {
    public:

        virtual ~entity();

    public:

        bool is_running() const;

        void listen_tick(const std::function<void()> &func);

        void refuse_to_listen_tick();

        void listen_touches_began(const std::function<bool(const std::vector<cc::float2> &)> &func);

        void listen_touches_moved(const std::function<void(const std::vector<cc::float2> &)> &func);

        void listen_touches_ended(const std::function<void(const std::vector<cc::float2> &)> &func);

        void listen_touches_cancelled(const std::function<void(const std::vector<cc::float2> &)> &func);

        void refuse_to_listen_touches();

        void refuse_to_listen_all();

        void preload(const std::vector<std::string> &paths);

        void play(const std::string &path);

        void stop(const std::string &path);

    protected:

        bool m_is_running = false;

    };

}