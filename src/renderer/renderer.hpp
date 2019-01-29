//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <vector>

#include "core/gl.hpp"
#include "core/singleton.hpp"
#include "renderer/vertex.hpp"

namespace cc {

    class sprite;

    class program;

    class render_area;

    struct float2;

    class renderer : public singleton<renderer> {
        friend class singleton<renderer>;

    public:

        void set_screen_size(const float2 &size);

        void add_sprite(cc::sprite *sprite);

        void update_render_area(const cc::quad &vertices);

        void render();

        void resume();

    private:

        struct batch {
            GLuint texture_name;
            size_t indices_count;
            size_t offset;
        };

    private:

        static const size_t RENDER_BUFFERS_COUNT = 3;
        static const int BUFFER_SIZE = 32768;

    private:

        std::vector<cc::sprite *> m_queue;

        cc::program *m_program = nullptr;
        cc::render_area *m_render_area;

        GLuint m_array_buffer;
        cc::vertex m_vertices_buffer[BUFFER_SIZE];

        GLuint m_element_array_buffer;
        GLushort m_indices_buffer[BUFFER_SIZE];

        std::vector<batch> m_batches;

    private:

        renderer();

        ~renderer();

    private:

        size_t fill_indices(GLushort *buffer, size_t start, size_t vertices_count);

        size_t fill_vertices(cc::vertex *buffer, cc::sprite *sprite);

        size_t fill_vertices_intersection(cc::vertex *buffer, cc::sprite *sprite, const std::vector<cc::float2> &intersection);

    };

}
