//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/gl.hpp"
#include "math/float2.hpp"
#include "math/int2.hpp"
#include "math/int4.hpp"

class stbrp_rect;

namespace cc {

    class frame;

    class texture {
    public:

        static std::shared_ptr<texture> create(const std::unordered_map<std::string, std::shared_ptr<cc::frame>> &frames, const bool has_mipmaps = false);

    public:

        texture(const std::unordered_map<std::string, std::shared_ptr<cc::frame>> &frames, const bool has_mipmaps);

        ~texture();

    public:

        void load();

        GLuint get_name() const;

        const cc::float2 &get_size() const;

        const cc::int4 &get_frame(const std::string &name);

        void check();

    protected:

        GLuint m_name = 0;

        cc::float2 m_size = {0.f, 0.f};

    private:

        static const int MAX_TEXTURE_SIZE = 4096;

    private:

        const bool m_has_mipmaps;

        std::unordered_map<std::string, std::shared_ptr<cc::frame>> m_frames;

    private:

        int pow2roundup(int x);

        cc::int2 get_texture_size(stbrp_rect *begin, stbrp_rect *end, const bool has_mipmaps);

        void set_data(const unsigned char *data, const cc::int2 &size, const bool has_mipmaps);


    };

}
