//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <unordered_map>

#include "core/audio.h"

@class AVAudioPlayer;

namespace cc {

    class audio_impl : public audio {
    public:

        ~audio_impl() override;

    public:

        void preload(const std::vector<std::string> &paths) override;

        void play(const std::string &path) override;

        void stop(const std::string &path) override;

    private:

        std::unordered_map<std::string, AVAudioPlayer *> m_players;

    };

}