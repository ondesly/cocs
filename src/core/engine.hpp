//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>

#include "core/singleton.hpp"

#include "math/float2.hpp"

namespace cc {

    class node;

    class renderer;

    class events;

    class device;

    class engine : public singleton<engine> {
        friend class singleton<engine>;

    public:

        void set_screen_size(const cc::float2 &size);

        const cc::float2 &get_screen_size() const;

        void run(std::shared_ptr<cc::node> node);

        bool is_running() const;

        void tick();

        void resume();

        void pause();

    private:

        bool m_paused = true;
        bool m_is_screen_size_changed = false;

        cc::renderer *m_renderer;
        cc::events *m_events;
        cc::device *m_device;

        cc::float2 m_screen_size;

        std::shared_ptr<cc::node> m_node;

    private:

        engine();

        ~engine();

    };

}