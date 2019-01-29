//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <cstddef>

#include "math/mat4.hpp"
#include "renderer/vertex.hpp"

#include "program.hpp"

const char *cc::program::FRAGMENT_SHADER = R"(
        precision lowp float;
        precision lowp int;

        uniform sampler2D texture;

        varying lowp vec4 fragmentColor;
        varying lowp vec2 textureCoords;

        void main()
        {
            gl_FragColor = fragmentColor * texture2D(texture, textureCoords);
        }
    )";

const char *cc::program::VERTEX_SHADER = R"(
        precision lowp float;
        precision lowp int;

        uniform mat4 projectionMat;

        attribute lowp vec4 positionAttribute;
        attribute lowp vec4 colorAttribute;
        attribute lowp vec2 textureCoordsAttribute;

        varying lowp vec4 fragmentColor;
        varying lowp vec2 textureCoords;

        void main()
        {
            gl_Position = projectionMat * positionAttribute;
            fragmentColor = colorAttribute;
            textureCoords = textureCoordsAttribute;
        }
    )";

const char *cc::program::UNIFORM_PROJECTION_MATRIX = "projectionMat";

const char *cc::program::ATTRIBUTE_POSITION = "positionAttribute";
const char *cc::program::ATTRIBUTE_COLOR = "colorAttribute";
const char *cc::program::ATTRIBUTE_TEXTURE_COORDS = "textureCoordsAttribute";

cc::program::program() {
    m_program = glCreateProgram();

    const auto vertex_shader = compile_and_attach_shader(m_program, GL_VERTEX_SHADER, VERTEX_SHADER);
    const auto fragment_shader = compile_and_attach_shader(m_program, GL_FRAGMENT_SHADER, FRAGMENT_SHADER);

    glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
    glBindAttribLocation(m_program, VERTEX_ATTRIB_POSITION, ATTRIBUTE_POSITION);
    glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *) offsetof(vertex, pos));

    glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
    glBindAttribLocation(m_program, VERTEX_ATTRIB_COLOR, ATTRIBUTE_COLOR);
    glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), (GLvoid *) offsetof(vertex, color));

    glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORDS);
    glBindAttribLocation(m_program, VERTEX_ATTRIB_TEXTURE_COORDS, ATTRIBUTE_TEXTURE_COORDS);
    glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *) offsetof(vertex, tex));

    glLinkProgram(m_program);
    glUseProgram(m_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void cc::program::set_projection_mat(const cc::mat4 &mat) {
    glUseProgram(m_program);
    glUniformMatrix4fv(glGetUniformLocation(m_program, UNIFORM_PROJECTION_MATRIX), 1, GL_FALSE, mat.value_ptr());
}

cc::program::~program() {
    glDeleteProgram(m_program);
}

GLuint cc::program::compile_and_attach_shader(const GLuint program, const GLenum type, const GLchar *source) {
    const auto shader = glCreateShader(type);
    glShaderSource(shader, sizeof(GLchar), &source, nullptr);
    glCompileShader(shader);
    glAttachShader(program, shader);

    return shader;
}
