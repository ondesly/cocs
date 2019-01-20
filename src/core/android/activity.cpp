//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <vector>

#include <android_native_app_glue.h>
#include <jni.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "core/android/audio_impl.h"
#include "core/android/files_impl.h"
#include "core/android/log_buffer.h"
#include "core/engine.h"
#include "core/touches.h"
#include "math/float2.h"
#include "textures/android/text_renderer_impl.h"

#include "activity.h"

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(cc::activity::activity_state *activity_state) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config = nullptr;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    assert(supportedConfigs);
    eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto &cfg = supportedConfigs[i];
        EGLint r, g, b, d, surfType = 0;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r) &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b) &&
            eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
            eglGetConfigAttrib(display, cfg, EGL_SURFACE_TYPE, &surfType) &&
            r == 8 && g == 8 && b == 8 && d == 0 && surfType & EGL_SWAP_BEHAVIOR_PRESERVED_BIT) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, activity_state->app->window, NULL);

    eglSurfaceAttrib(display, surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

    EGLint AttribList[] =
            {
                    EGL_CONTEXT_CLIENT_VERSION, 2,
                    EGL_NONE
            };

    context = eglCreateContext(display, config, NULL, AttribList);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    activity_state->display = display;
    activity_state->context = context;
    activity_state->surface = surface;
    activity_state->width = w;
    activity_state->height = h;

    //

    new cc::engine(cc::float2(w, h),
                   new cc::files_impl(activity_state->app->activity->assetManager),
                   new cc::text_renderer_impl(activity_state->app->activity),
                   new cc::audio_impl(activity_state->app->activity));
    cc::engine::i()->run(activity_state->make_scene_func());

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(cc::activity::activity_state *activity_state) {
    if (activity_state->display == NULL) {
        // No display.
        return;
    }

    cc::engine::i()->tick();

    eglSwapBuffers(activity_state->display, activity_state->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(cc::activity::activity_state *activity_state) {
    if (activity_state->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(activity_state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (activity_state->context != EGL_NO_CONTEXT) {
            eglDestroyContext(activity_state->display, activity_state->context);
        }
        if (activity_state->surface != EGL_NO_SURFACE) {
            eglDestroySurface(activity_state->display, activity_state->surface);
        }
        eglTerminate(activity_state->display);
    }
    activity_state->animating = 0;
    activity_state->display = EGL_NO_DISPLAY;
    activity_state->context = EGL_NO_CONTEXT;
    activity_state->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app *app, AInputEvent *event) {
    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION) {
        return 0;
    }

    std::vector<cc::float2> touches;
    auto count = AMotionEvent_getPointerCount(event);
    for (size_t i = 0; i < count; ++i) {
        touches.push_back({AMotionEvent_getX(event, i), AMotionEvent_getY(event, i)});
    }

    int action = AMotionEvent_getAction(event);
    int flags = action & AMOTION_EVENT_ACTION_MASK;
    switch (flags) {
        case AMOTION_EVENT_ACTION_DOWN:
            cc::engine::i()->get_touches()->dispatch_touches_began(touches);
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            cc::engine::i()->get_touches()->dispatch_touches_moved(touches);
            break;
        case AMOTION_EVENT_ACTION_UP:
            cc::engine::i()->get_touches()->dispatch_touches_ended(touches);
            break;
        case AMOTION_EVENT_ACTION_CANCEL:
            cc::engine::i()->get_touches()->dispatch_touches_cancelled(touches);
            break;
        default:
            break;
    }

    return 1;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
    auto activity_state = (cc::activity::activity_state *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (activity_state->app->window != NULL) {
                engine_init_display(activity_state);
                engine_draw_frame(activity_state);

                activity_state->animating = 1;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(activity_state);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            activity_state->animating = 1;
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            // Also stop animating.
            activity_state->animating = 0;
            engine_draw_frame(activity_state);
            break;
        default:
            break;
    }
}

cc::activity::activity(struct android_app *state) {
    std::cout.rdbuf(new log_buffer);

    memset(&m_activity_state, 0, sizeof(m_activity_state));
    state->userData = &m_activity_state;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    m_activity_state.app = state;
}

void cc::activity::run(const std::function<std::shared_ptr<cc::node>()> &make_scene_func) {
    m_activity_state.make_scene_func = make_scene_func;

    while (1) {
        // Read all pending events.
        int events;
        struct android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ALooper_pollAll(m_activity_state.animating ? 0 : -1, NULL, &events,
                                (void **) &source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(m_activity_state.app, source);
            }

            // Check if we are exiting.
            if (m_activity_state.app->destroyRequested != 0) {
                engine_term_display(&m_activity_state);
                return;
            }
        }

        if (m_activity_state.animating) {
            engine_draw_frame(&m_activity_state);
        }
    }
}
