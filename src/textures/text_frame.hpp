//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>
#include <string>

#include "math/float2.hpp"
#include "textures/frame.hpp"

namespace cc {

    class text_frame : public frame {
    public:

        static std::shared_ptr<text_frame> make(const std::string &text, float font_size, float max_width = 0.f);

    public:

        text_frame(const std::string &text, float font_size, float max_width);

        ~text_frame() override;

        void clear() override;

    public:

        void load() override;

        void render(unsigned char *data, const cc::int2 &data_size) override;

    private:

        std::string m_text;
        float m_font_size;
        float m_max_width;

        unsigned char *m_data = nullptr;

    };

}