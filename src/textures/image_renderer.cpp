//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <cmath>

#include <cairo/cairo.h>
#include <pugixml/pugixml.hpp>

#include "math/float2.hpp"
#include "math/int4.hpp"

#include "image_renderer.hpp"

cc::image_renderer::image_renderer(unsigned char *data, int width, int height) {
    auto surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, width, height, width * 4);
    m_cr = cairo_create(surface);
    cairo_surface_destroy(surface);
}

cc::image_renderer::~image_renderer() {
    cairo_destroy(m_cr);
}

void cc::image_renderer::render(const pugi::xml_node &root, const cc::int4 &rect, float scale, const cc::float2 &anchor) {
    cairo_save(m_cr);

    //

    cairo_move_to(m_cr, rect.x, rect.y);
    cairo_rel_line_to(m_cr, rect.w, 0.);
    cairo_rel_line_to(m_cr, 0., rect.h);
    cairo_rel_line_to(m_cr, -rect.w, 0.);
    cairo_clip(m_cr);

    cairo_translate(m_cr, rect.x + anchor.x, rect.y + anchor.y);
    cairo_scale(m_cr, scale, scale);

    // Render

    for (auto &node : root.children()) {
        if (strcmp(node.attribute("fill").as_string(), "none") == 0) {
            continue;
        }

        //

        if (std::strcmp(node.name(), "path") == 0) {
            render_path(node.attribute("d").as_string());
        } else if (std::strcmp(node.name(), "rect") == 0) {
            render_rect(node.attribute("x").as_float(), node.attribute("y").as_float(),
                    node.attribute("width").as_float(), node.attribute("height").as_float());
        } else if (std::strcmp(node.name(), "circle") == 0) {
            render_circle(node.attribute("cx").as_float(), node.attribute("cy").as_float(),
                    node.attribute("r").as_float());
        } else if (std::strcmp(node.name(), "ellipse") == 0) {
            render_ellipse(node.attribute("cx").as_float(), node.attribute("cy").as_float(),
                    node.attribute("rx").as_float(), node.attribute("ry").as_float());
        } else {
            continue;
        }

        //

        fill(node.attribute("fill").as_string(nullptr), node.attribute("opacity").as_string(nullptr));
    }

    cairo_restore(m_cr);
}

void cc::image_renderer::render(pugi::xml_node &root, int width, int height, float box_width, float box_height, const cc::float2 &anchor) {

    // Translate

    float2 actual_size;
    if (width / box_width > height / box_height) {
        actual_size = {float(width), box_height * width / box_width};
    } else {
        actual_size = {box_width * height / box_height, float(height)};
    }

    cairo_translate(m_cr, (width - actual_size.w) * anchor.x, (height - actual_size.h) * anchor.y);

    // Scale

    float scale = std::max(width / box_width, height / box_height);
    cairo_scale(m_cr, scale, scale);

    // Render

    for (auto &node : root.children()) {
        if (strcmp(node.attribute("fill").as_string(), "none") == 0) {
            continue;
        }

        //

        if (std::strcmp(node.name(), "path") == 0) {
            render_path(node.attribute("d").as_string());
        } else if (std::strcmp(node.name(), "rect") == 0) {
            render_rect(node.attribute("x").as_float(), node.attribute("y").as_float(),
                    node.attribute("width").as_float(), node.attribute("height").as_float());
        } else if (std::strcmp(node.name(), "circle") == 0) {
            render_circle(node.attribute("cx").as_float(), node.attribute("cy").as_float(),
                    node.attribute("r").as_float());
        } else if (std::strcmp(node.name(), "ellipse") == 0) {
            render_ellipse(node.attribute("cx").as_float(), node.attribute("cy").as_float(),
                    node.attribute("rx").as_float(), node.attribute("ry").as_float());
        } else {
            continue;
        }

        //

        fill(node.attribute("fill").as_string(nullptr), node.attribute("opacity").as_string(nullptr));
    }
}

void cc::image_renderer::read(const char *&src, double *dst, double *dst2) {
    char *end;
    while (dst <= dst2) {
        *dst = std::strtod(src, &end);
        ++dst;

        src = *end == ',' ? end + 1 : end;
    }
}

void cc::image_renderer::read(const char *&src, long *dst, long *dst2) {
    char *end;
    while (dst <= dst2) {
        *dst = std::strtol(src, &end, 10);
        ++dst;

        src = *end == ',' ? end + 1 : end;
    }
}

void cc::image_renderer::update_parameters_by_previous(const char prev_cmd, cc::image_renderer::params &params,
        const double x0, const double y0) {
    switch (prev_cmd) {
        case command::ABSOLUTE_CURVE:
        case command::ABSOLUTE_SMOOTH_CURVE:
            params.x1 = -(params.x2 - params.x0) + x0;
            params.y1 = -(params.y2 - params.y0) + y0;
            break;
        case command::RELATIVE_CURVE:
        case command::RELATIVE_SMOOTH_CURVE:
            params.x1 = -(params.x2 - params.x3) + x0;
            params.y1 = -(params.y2 - params.y3) + y0;
            break;
        case ABSOLUTE_QUADRATIC_CURVE:
        case ABSOLUTE_TOGETHER_CURVE:
            params.x1 = -(params.x1 - params.x0) + x0;
            params.y1 = -(params.y1 - params.y0) + y0;
            break;
        case RELATIVE_QUADRATIC_CURVE:
        case RELATIVE_TOGETHER_CURVE:
            params.x1 = -(params.x1 - params.x3) + x0;
            params.y1 = -(params.y1 - params.y3) + y0;
            break;
        default:
            params.x1 = x0;
            params.y1 = y0;
            break;
    }
}

void cc::image_renderer::update_parameters(const char cmd, const char prev_cmd, cc::image_renderer::params &params, const char *&src) {
    cairo_get_current_point(m_cr, &params.x0, &params.y0);

    switch (cmd) {
        case command::ABSOLUTE_MOVE:
        case command::RELATIVE_MOVE:
        case command::ABSOLUTE_LINE:
        case command::RELATIVE_LINE:
            read(src, &params.x, &params.y);
            break;
        case command::ABSOLUTE_HORIZONTAL:
        case command::RELATIVE_HORIZONTAL:
            read(src, &params.x, &params.x);
            break;
        case command::ABSOLUTE_VERTICAL:
        case command::RELATIVE_VERTICAL:
            read(src, &params.y, &params.y);
            break;
        case command::ABSOLUTE_CURVE:
        case command::RELATIVE_CURVE:
            read(src, &params.x1, &params.y3);
            break;
        case command::ABSOLUTE_SMOOTH_CURVE:
            update_parameters_by_previous(prev_cmd, params, params.x0, params.y0);
            read(src, &params.x2, &params.y3);
            break;
        case command::RELATIVE_SMOOTH_CURVE:
            update_parameters_by_previous(prev_cmd, params, 0., 0.);
            read(src, &params.x2, &params.y3);
            break;
        case command::ABSOLUTE_QUADRATIC_CURVE:
        case command::RELATIVE_QUADRATIC_CURVE:
            read(src, &params.x1, &params.y1);
            read(src, &params.x3, &params.y3);
            break;
        case command::ABSOLUTE_TOGETHER_CURVE:
            update_parameters_by_previous(prev_cmd, params, params.x0, params.y0);
            read(src, &params.x3, &params.y3);
            break;
        case command::RELATIVE_TOGETHER_CURVE:
            update_parameters_by_previous(prev_cmd, params, 0., 0.);
            read(src, &params.x3, &params.y3);
            break;
        case command::ABSOLUTE_ARC:
            read(src, &params.rx, &params.rotation);
            read(src, &params.large_arc_flag, &params.sweep_flag);
            read(src, &params.x, &params.y);

            params.x -= params.x0;
            params.y -= params.y0;
            break;
        case command::RELATIVE_ARC:
            read(src, &params.rx, &params.rotation);
            read(src, &params.large_arc_flag, &params.sweep_flag);
            read(src, &params.x, &params.y);
            break;
        default:
            break;
    }
}

void cc::image_renderer::render_path(const char *src) {
    cc::image_renderer::params params{};
    char cmd = 0;
    char prev_cmd = 0;

    while (*src != 0) {
        if (*src - 'A' >= 0) {
            cmd = *src;
            ++src;
        }

        update_parameters(cmd, prev_cmd, params, src);

        switch (cmd) {
            case command::ABSOLUTE_MOVE:
                cairo_move_to(m_cr, params.x, params.y);
                break;
            case command::RELATIVE_MOVE:
                cairo_rel_move_to(m_cr, params.x, params.y);
                break;
            case command::ABSOLUTE_LINE:
                cairo_line_to(m_cr, params.x, params.y);
                break;
            case command::RELATIVE_LINE:
                cairo_rel_line_to(m_cr, params.x, params.y);
                break;
            case command::ABSOLUTE_HORIZONTAL:
                cairo_line_to(m_cr, params.x, params.y0);
                break;
            case command::RELATIVE_HORIZONTAL:
                cairo_rel_line_to(m_cr, params.x, 0.);
                break;
            case command::ABSOLUTE_VERTICAL:
                cairo_line_to(m_cr, params.x0, params.y);
                break;
            case command::RELATIVE_VERTICAL:
                cairo_rel_line_to(m_cr, 0., params.y);
                break;
            case command::ABSOLUTE_CURVE:
                cairo_curve_to(m_cr, params.x1, params.y1, params.x2, params.y2, params.x3, params.y3);
                break;
            case command::RELATIVE_CURVE:
                cairo_rel_curve_to(m_cr, params.x1, params.y1, params.x2, params.y2, params.x3, params.y3);
                break;
            case command::ABSOLUTE_SMOOTH_CURVE:
                cairo_curve_to(m_cr, params.x1, params.y1, params.x2, params.y2, params.x3, params.y3);
                break;
            case command::RELATIVE_SMOOTH_CURVE:
                cairo_rel_curve_to(m_cr, params.x1, params.y1, params.x2, params.y2, params.x3, params.y3);
                break;
            case command::ABSOLUTE_QUADRATIC_CURVE:
            case command::ABSOLUTE_TOGETHER_CURVE:
                cairo_curve_to(m_cr,
                        2. / 3. * params.x1 + 1. / 3. * params.x0, 2. / 3. * params.y1 + 1. / 3. * params.y0,
                        2. / 3. * params.x1 + 1. / 3. * params.x3, 2. / 3. * params.y1 + 1. / 3. * params.y3,
                        params.x3, params.y3);
                break;
            case command::RELATIVE_QUADRATIC_CURVE:
            case command::RELATIVE_TOGETHER_CURVE:
                cairo_rel_curve_to(m_cr,
                        2. / 3. * params.x1, 2. / 3. * params.y1,
                        2. / 3. * params.x1 + 1. / 3. * params.x3, 2. / 3. * params.y1 + 1. / 3. * params.y3,
                        params.x3, params.y3);
                break;
            case command::ABSOLUTE_ARC:
            case command::RELATIVE_ARC:
                arc(params.rx, params.ry, params.rotation, params.large_arc_flag, params.sweep_flag,
                        params.x, params.y, params.x0, params.y0);
                break;
            case command::ABSOLUTE_CLOSE:
            case command::RELATIVE_CLOSE:
                cairo_close_path(m_cr);
                break;
            default:
                break;
        }

        prev_cmd = cmd;
    }
}

void cc::image_renderer::render_rect(const float x, const float y, const float w, const float h) {
    cairo_move_to(m_cr, x, y);

    cairo_rel_line_to(m_cr, w, 0.);
    cairo_rel_line_to(m_cr, 0., h);
    cairo_rel_line_to(m_cr, -w, 0.);

    cairo_close_path(m_cr);
}

void cc::image_renderer::render_circle(const float cx, const float cy, const float r) {
    cairo_move_to(m_cr, cx - r, cy);

    double x0, y0;
    cairo_get_current_point(m_cr, &x0, &y0);
    arc(r, r, 0., 1, 0, r + r, 0., x0, y0);
    cairo_get_current_point(m_cr, &x0, &y0);
    arc(r, r, 0., 1, 0, -(r + r), 0., x0, y0);

    cairo_close_path(m_cr);
}

void cc::image_renderer::render_ellipse(const float cx, const float cy, const float rx, const float ry) {
    cairo_move_to(m_cr, cx - rx, cy);

    double x0, y0;
    cairo_get_current_point(m_cr, &x0, &y0);
    arc(rx, ry, 0., 1, 0, rx + ry, 0., x0, y0);
    cairo_get_current_point(m_cr, &x0, &y0);
    arc(rx, ry, 0., 1, 0, -(rx + ry), 0., x0, y0);

    cairo_close_path(m_cr);
}

void cc::image_renderer::transform(const double angle, double x, double y, double &dst_x, double &dst_y) {
    dst_x = x * std::cos(angle) - y * std::sin(angle);
    dst_y = y * std::cos(angle) + x * std::sin(angle);
}

void cc::image_renderer::arc(double rx, double ry, double rotation, long large_arc_flag, long sweep_flag,
        double x, double y, double x0, double y0) {
    if (rx <= 0 || ry / rx <= 0) {
        return;
    }

    transform(-rotation * 0.017453292519943, x, y, x, y);

    y /= ry / rx;
    auto angle = std::atan2(y, x);
    x = std::sqrt(x * x + y * y);

    auto xc = x / 2;
    auto rxc = std::max(rx, xc);
    auto yc = std::sqrt(rxc * rxc - xc * xc) * (large_arc_flag ^ sweep_flag ? 1 : -1);

    transform(angle, xc, yc, xc, yc);
    transform(angle, x, 0., x, y);

    //

    cairo_save(m_cr);

    cairo_translate(m_cr, x0, y0);
    cairo_rotate(m_cr, rotation * 0.017453292519943);
    cairo_scale(m_cr, 1, ry / rx);

    if (sweep_flag) {
        cairo_arc(m_cr, xc, yc, rxc, std::atan2(-yc, -xc), std::atan2(y - yc, x - xc));
    } else {
        cairo_arc_negative(m_cr, xc, yc, rxc, std::atan2(-yc, -xc), std::atan2(y - yc, x - xc));
    }

    cairo_restore(m_cr);
}

void cc::image_renderer::fill(const char *fill, const char *opacity) {
    double a = opacity ? atof(opacity) : 1.;

    double r;
    double g;
    double b;
    if (fill) {
        char c_r[3], c_g[3], c_b[3];

        if (strlen(fill) == 7) {
            std::sscanf(fill, "#%2s%2s%2s", c_r, c_g, c_b);
        } else {
            std::sscanf(fill, "#%1s%1s%1s", c_r, c_g, c_b);
            std::sscanf(fill, "#%1s%1s%1s", &c_r[1], &c_g[1], &c_b[1]);
        }

        r = std::strtol(c_r, nullptr, 16) / 255.;
        g = std::strtol(c_g, nullptr, 16) / 255.;
        b = std::strtol(c_b, nullptr, 16) / 255.;
    } else {
        r = g = b = 0.;
    }

    cairo_set_source_rgba(m_cr, b, g, r, a);
    cairo_fill(m_cr);
}