//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//
#pragma once

#include <functional>
#include <memory>

#include <EGL/egl.h>

struct android_app;

namespace cc {

    class node;

    class activity {
    public:

        struct activity_state {

            struct android_app *app;

            int animating;
            EGLDisplay display;
            EGLSurface surface;
            EGLContext context;
            int32_t width;
            int32_t height;

            std::function<std::shared_ptr<cc::node>()> make_scene_func;

        };

    public:

        activity(struct android_app *state);

    public:

        void run(const std::function<std::shared_ptr<cc::node>()> &make_scene_func);

    private:

        activity_state m_activity_state;

    };

}