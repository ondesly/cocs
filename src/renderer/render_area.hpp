//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <deque>
#include <vector>

#include "math/float4.hpp"

namespace cc {

    struct float2;

    struct quad;

    class render_area {
    public:

        enum class status {
            OUTSIDE,
            INSIDE,
            INTERSECTION
        };

    public:

        explicit render_area(size_t render_buffers_count);

    public:

        void add(const cc::quad &vertices);

        bool is_changed() const;

        void update();

        void done();

        void reset();

        cc::render_area::status get_intersection_status(const cc::quad &vertices) const;

        std::vector<cc::float2> get_intersection(const cc::quad &vertices);

    private:

        static const int MAX_VERTICES_COUNT = 256;

    private:

        size_t m_render_buffers_count;

        std::deque<float> m_lefts;
        std::deque<float> m_rights;
        std::deque<float> m_tops;
        std::deque<float> m_bottoms;

        cc::float4 m_area;

        float m_xs[MAX_VERTICES_COUNT];
        float m_ys[MAX_VERTICES_COUNT];
        int m_vertices_count = 0;

        size_t m_frames_from_last_change = 0;
        size_t m_frames_to_skip = 0;

    };

}