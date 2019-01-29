//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

#include "core/singleton.hpp"
#include "math/float2.hpp"

namespace cc {

    class device : public singleton<device> {
        friend class singleton<device>;

    public:

        device();

        ~device();

    public:

        void load_music(const std::string &path);

        void play_music();

        void pause_music();

        void stop_music();

        void clear_music();

        void load_sounds(const std::vector<std::string> &paths);

        void play_sound(const std::string &path);

        void clear_sounds();

        std::string get_file_content(const std::string &path);

        cc::float2 render_text(const std::string &value, float font_size, float max_width, unsigned char *&data);

    };

}