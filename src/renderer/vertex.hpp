//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include "math/float2.hpp"
#include "math/color4.hpp"

namespace cc {

    struct vertex {

        cc::float2 pos;
        cc::color4 color;
        cc::float2 tex;

    };

    struct quad {

        cc::vertex tl;
        cc::vertex bl;
        cc::vertex br;
        cc::vertex tr;

    };

}