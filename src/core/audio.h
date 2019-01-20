//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>
#include <vector>

namespace cc {

    class audio {
    public:

        virtual ~audio() = default;

    public:

        virtual void preload(const std::vector<std::string> &paths) = 0;

        virtual void play(const std::string &path) = 0;

        virtual void stop(const std::string &path) = 0;

    };

}