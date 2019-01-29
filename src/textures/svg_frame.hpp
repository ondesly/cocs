//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>
#include <string>

#include "math/float2.hpp"
#include "textures/frame.hpp"

namespace pugi {

    class xml_document;

}

namespace cc {

    class svg_frame : public frame {
    public:

        static std::shared_ptr<svg_frame> make(const std::string &path, const cc::float2 &size, const cc::float2 &anchor = {0.f, 0.f});

    public:

        svg_frame(const std::string &path, const cc::float2 &size, const cc::float2 &anchor);

        ~svg_frame() override;

    public:

        void load() override;

        void render(unsigned char *data, const cc::int2 &data_size) override;

        void clear() override;

    private:

        std::string m_path;
        cc::float2 m_size;
        cc::float2 m_anchor;

        float m_scale;
        cc::float2 m_anchor_in_size;
        pugi::xml_document *m_doc = nullptr;

    private:

        cc::float2 get_original_size(const char *s);

        cc::float2 get_frame_size(const cc::float2 &size, const cc::float2 &original_size);

        cc::float2 get_actual_size(const cc::float2 &frame_size, const cc::float2 &original_size);

        pugi::xml_document *read_image(const std::string &path);

    };

}