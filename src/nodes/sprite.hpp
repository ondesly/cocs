//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

#include "nodes/node.hpp"
#include "renderer/vertex.hpp"

namespace cc {

    class texture;

    class frame;

    class renderer;

    struct mat4;

    class sprite : public node {
    public:

        static std::shared_ptr<sprite> create(const std::shared_ptr<cc::texture> &texture, const std::string &frame_name = "");

    public:

        sprite(const std::shared_ptr<cc::texture> &texture, const std::string &frame_name);

        sprite(const sprite &) = delete;

        sprite &operator=(const sprite &) = delete;

    public:

        void resume() override;

        void on_exit() override;

        void draw() override;

        std::shared_ptr<cc::texture> get_texture() const;

        const cc::quad &get_vertices() const;

    protected:

        void update_color() override;

        void update_transform(const cc::mat4 &transform) override;

    private:

        cc::renderer *m_renderer;
        std::shared_ptr<cc::texture> m_texture;

        cc::quad m_vertices = {
                {{0.f, 0.f}, {255, 255, 255, 255}, {0.f, 0.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {0.f, 1.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {1.f, 1.f}},
                {{0.f, 0.f}, {255, 255, 255, 255}, {1.f, 0.f}}};

    };

}
