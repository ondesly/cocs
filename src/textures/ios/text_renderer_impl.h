//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include "textures/text_renderer.h"

namespace cc {

    class text_renderer_impl : public cc::text_renderer {
    public:

        cc::float2 render(const std::string &value, const float font_size, float max_width, unsigned char *&data) override;

    };

}