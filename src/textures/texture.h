//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "math/float2.h"
#include "math/float4.h"
#include "core/gl.h"

namespace pugi {

    class xml_document;

}

namespace cc {

    class texture {
    public:

        struct params {

            params(const std::string &path, const std::string &name, const cc::float2 &size, const cc::float2 &anchor = {0.f, 0.f})
                    : path(path), name(name), size(size), anchor(anchor), is_font(false) {

            }

            params(const std::string &text, const std::string &name, const float font_size, const float max_width = 0.f)
                    : text(text), name(name), size(max_width, font_size), is_font(true) {

            }

            std::string name;
            cc::float2 size;
            bool is_font;

            std::string path;
            cc::float2 anchor;

            std::string text;

        };

    public:

        static std::shared_ptr<texture> create(const std::vector<params> &frames);

    public:

        texture(const std::vector<params> &frames);

        ~texture();

    public:

        GLuint get_name() const;

        const cc::float2 &get_size() const;

        const cc::float4 &get_frame(const std::string &name);

    protected:

        GLuint m_name = 0;

        cc::float2 m_size = {0.f, 0.f};

    private:

        struct atlas_params {
            std::string name;
            pugi::xml_document *doc;
            const unsigned char *data;
            float scale;
            float2 anchor;
        };

    private:

        std::unordered_map<std::string, cc::float4> m_frames;

    private:

        void set_data(const unsigned char *data, const int width, const int height);

    };

}
