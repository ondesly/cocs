//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

#include "nodes/node.h"
#include "renderer/vertex.h"

namespace cc {

    class texture;

    class frame;

    class renderer;

    struct mat4;

    class sprite : public node {
    public:

        static std::shared_ptr<sprite> create(std::shared_ptr<cc::texture> texture, const std::string &frame_name = "");

    public:

        sprite(std::shared_ptr<cc::texture> texture, const std::string &frame_name);

    public:

        void draw(renderer *renderer) override;

        std::shared_ptr<cc::texture> get_texture() const;

        const cc::quad &get_vertices() const;

    protected:

        void update_color() override;

        void update_transform(cc::renderer *const renderer, const cc::mat4 &transform) override;

    private:

        std::shared_ptr<cc::texture> m_texture;

        cc::quad m_vertices = {
                {{0.f, 0.f}, {255, 255, 255, 255}, {0.f, 0.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {0.f, 1.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {1.f, 1.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {1.f, 0.f}}};

    private:

        sprite(const sprite &) = delete;

        sprite &operator=(const sprite &) = delete;

    };

}
