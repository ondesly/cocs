//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

struct _cairo;
typedef struct _cairo cairo_t;

namespace pugi {

    class xml_node;

}

namespace cc {

    struct float2;

    struct float4;

    class image_renderer {
    public:

        image_renderer(unsigned char *data, int width, int height);

        ~image_renderer();

        void render(const pugi::xml_node &root, const cc::float4 &rect, float scale, const cc::float2 &anchor);

        void render(pugi::xml_node &root, int width, int height, float box_width, float box_height, const cc::float2 &anchor);

    private:

        enum command : char {
            ABSOLUTE_MOVE = 'M',
            RELATIVE_MOVE = 'm',
            ABSOLUTE_LINE = 'L',
            RELATIVE_LINE = 'l',
            ABSOLUTE_HORIZONTAL = 'H',
            RELATIVE_HORIZONTAL = 'h',
            ABSOLUTE_VERTICAL = 'V',
            RELATIVE_VERTICAL = 'v',
            ABSOLUTE_CURVE = 'C',
            RELATIVE_CURVE = 'c',
            ABSOLUTE_SMOOTH_CURVE = 'S',
            RELATIVE_SMOOTH_CURVE = 's',
            ABSOLUTE_QUADRATIC_CURVE = 'Q',
            RELATIVE_QUADRATIC_CURVE = 'q',
            ABSOLUTE_TOGETHER_CURVE = 'T',
            RELATIVE_TOGETHER_CURVE = 't',
            ABSOLUTE_ARC = 'A',
            RELATIVE_ARC = 'a',
            ABSOLUTE_CLOSE = 'Z',
            RELATIVE_CLOSE = 'z'
        };

    private:

        struct params {
            double x0;
            double y0;
            double x1;
            double y1;
            double x2;
            double y2;
            double x3;
            double y3;

            double rx;
            double ry;
            double rotation;
            long large_arc_flag;
            long sweep_flag;

            double x;
            double y;
        };

    private:

        cairo_t *m_cr;

    private:

        void render_path(const char *src);

        void render_rect(const float x, const float y, const float w, const float h);

        void render_circle(const float cx, const float cy, const float r);

        void render_ellipse(const float cx, const float cy, const float rx, const float ry);

        void transform(const double angle, double x, double y, double &dst_x, double &dst_y);

        void arc(double rx, double ry, double rotation, long large_arc_flag, long sweep_flag,
                double x, double y, double x0, double y0);

        void fill(const char *fill, const char *opacity);

        void read(const char *&src, double *dst, double *dst2);

        void read(const char *&src, long *dst, long *dst2);

        void update_parameters_by_previous(const char prev_cmd, params &params, const double x0, const double y0);

        void update_parameters(const char cmd, const char prev_cmd, params &params, const char *&src);

    };

}