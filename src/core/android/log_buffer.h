//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <iosfwd>

namespace cc {

    class log_buffer : public std::streambuf {
    public:

        log_buffer();

    private:

        static const char *TAG;

        static const int BUFFER_SIZE = 128;

    private:

        char buffer[BUFFER_SIZE];

    private:

        int overflow(const int c);

        int sync();

    };

}