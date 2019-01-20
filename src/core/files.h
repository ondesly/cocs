//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

namespace cc {

    class files {
    public:

        virtual ~files() = default;

    public:

        virtual std::string get_content(const std::string &path) = 0;

    };

}