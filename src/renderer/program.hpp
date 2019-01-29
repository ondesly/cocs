//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include "core/gl.hpp"

namespace cc {

    struct mat4;

    class program {
    public:

        program();

        ~program();

    public:

        void set_projection_mat(const cc::mat4 &mat);

    private:

        enum {
            VERTEX_ATTRIB_POSITION = 0,
            VERTEX_ATTRIB_COLOR,
            VERTEX_ATTRIB_TEXTURE_COORDS,
        };

        static const char *UNIFORM_PROJECTION_MATRIX;

        static const char *ATTRIBUTE_COLOR;
        static const char *ATTRIBUTE_POSITION;
        static const char *ATTRIBUTE_TEXTURE_COORDS;

        static const char *FRAGMENT_SHADER;
        static const char *VERTEX_SHADER;

    private:

        GLuint m_program;

    private:

        GLuint compile_and_attach_shader(GLuint program, GLenum type, const GLchar *source);

    };

}