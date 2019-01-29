//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
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

            cc::activity *activity;

            struct android_app *app;

            bool animating;
            EGLDisplay display = EGL_NO_DISPLAY;
            EGLSurface surface = EGL_NO_SURFACE;
            EGLContext context = EGL_NO_CONTEXT;
            EGLConfig config;
            EGLint width;
            EGLint height;

        };

    public:

        explicit activity(struct android_app *state);

        virtual ~activity() = default;

    public:

        virtual std::shared_ptr<cc::node> make_scene() = 0;

        void run();

    private:

        activity_state m_activity_state;

    };

}