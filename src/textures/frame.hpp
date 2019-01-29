//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include "math/int4.hpp"

class stbrp_rect;

namespace cc {

    struct int2;

    class frame {
    public:

        virtual ~frame() = default;

    public:

        virtual void load() = 0;

        virtual void render(unsigned char *data, const cc::int2 &data_size) = 0;

        virtual void clear() = 0;

        void set_pack_rect(stbrp_rect *rect);

        void update_rect();

        const cc::int4 &get_rect();

    protected:

        cc::int4 m_rect;

        stbrp_rect *m_pack_rect;

    private:

        static const int PADDING = 1;

    };

}