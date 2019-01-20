//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>

#include "math/float2.h"

namespace cc {

    class node;

    class scheduler;

    class renderer;

    class touches;

    class files;

    class text_renderer;

    class audio;

    class engine {
    public:

        static cc::engine *i();

    public:

        engine(const cc::float2 &size, cc::files *const files, cc::text_renderer *const fonts, cc::audio *const audio);

        ~engine();

    public:

        const cc::float2 &get_screen_size() const;

        void run(std::shared_ptr<cc::node> node);

        void tick();

        cc::scheduler *get_scheduler() const;

        cc::touches *get_touches() const;

        cc::files *get_files();

        cc::text_renderer *get_text_renderer();

        cc::audio *get_audio();

    private:

        static cc::engine *m_instance;

    private:

        cc::renderer *m_renderer;

        cc::scheduler *m_scheduler;
        cc::touches *m_touches;
        cc::files *m_files;
        cc::text_renderer *m_text_renderer;
        cc::audio *m_audio;

        cc::float2 m_screen_size;

        std::shared_ptr<cc::node> m_node;

    };

}