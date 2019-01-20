//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

#include "math/float2.h"

namespace cc {

    class text_renderer {
    public:

        virtual ~text_renderer() = default;

    public:

        virtual cc::float2 render(const std::string &value, const float font_size, float max_width, unsigned char *&data) = 0;

    };

}