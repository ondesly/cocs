//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <unordered_map>

#include "core/audio.h"

class ANativeActivity;

struct AAssetManager;

class _jobject;

typedef _jobject *jobject;

namespace cc {

    class jvm;

    class audio_impl : public audio {
    public:

        audio_impl(ANativeActivity *const activity);

        ~audio_impl() override;

    public:

        void preload(const std::vector<std::string> &paths) override;

        void play(const std::string &path) override;

        void stop(const std::string &path) override;

    private:

        jvm *m_jvm;

        AAssetManager *m_asset_manager;

        std::unordered_map<std::string, jobject> m_players;

    };

}