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

    struct int2;

    class empty_frame : public frame {
    public:

        static std::shared_ptr<empty_frame> make(const cc::float2 &size);

    public:

        explicit empty_frame(const cc::float2 &size);

    public:

        void load() override;

        void render(unsigned char *data, const cc::int2 &data_size) override;

        void clear() override;

    private:

        cc::float2 m_size;

    };

}