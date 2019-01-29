//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/macros.hpp"
#include "math/color4.hpp"
#include "math/mat4.hpp"

namespace cc {

    class value;

    class node {
    public:

        static const int TICK = -1;
        static const int TOUCH_BEGAN = TICK - 1;
        static const int TOUCH_MOVED = TOUCH_BEGAN - 1;
        static const int TOUCH_ENDED = TOUCH_MOVED - 1;
        static const int TOUCH_CANCELLED = TOUCH_ENDED - 1;

    public:

        static std::shared_ptr<node> create();

    public:

        node() = default;

        node(const node &) = delete;

        node &operator=(const node &) = delete;

        virtual ~node();

    public:

        virtual void set_scale(float scale);

        virtual void set_scale(const cc::float2 &scale);

        virtual const cc::float2 &get_scale() const;

        virtual void set_position(const cc::float2 &pos);

        virtual const cc::float2 &get_position() const;

        virtual void set_anchor(const cc::float2 &anchor);

        virtual const cc::float2 &get_anchor() const;

        virtual void set_size(const cc::float2 &size);

        virtual const cc::float2 &get_size() const;

        virtual void set_visible(bool visible);

        virtual bool is_visible() const;

        virtual void set_rotation(float rotation);

        virtual float get_rotation() const;

        virtual void add_child(const std::shared_ptr<cc::node> &child);

        virtual void remove_child(const std::shared_ptr<cc::node> &child);

        virtual void remove_children();

        virtual std::vector<std::shared_ptr<cc::node>> &get_children();

        virtual const std::vector<std::shared_ptr<cc::node>> &get_children() const;

        virtual void set_parent(cc::node *parent);

        virtual cc::node *get_parent();

        virtual const node *get_parent() const;

        virtual const cc::color4 &get_color() const;

        virtual void set_color(const cc::color4 &color);

        virtual void visit(const cc::mat4 &parent_transform, bool is_parent_dirty);

        virtual void resume();

        virtual void pause();

        virtual void on_enter();

        virtual void on_exit();

        virtual void on_size_changed();

        bool is_running() const;

        bool is_contains(const cc::float2 &point);

        void listen(int id, const std::function<void(const cc::value &)> &fn);

        void stop_listening(int id = 0);

    protected:

        bool m_is_visible = true;
        bool m_is_running = false;

        cc::float2 m_position = {0.f, 0.f};
        cc::float2 m_anchor = {0.f, 0.f};
        cc::float2 m_anchor_in_size = {0.f, 0.f};
        cc::float2 m_scale = {1.f, 1.f};
        float m_rotation = 0.f;

        cc::float2 m_size = {0.f, 0.f};

        cc::mat4 m_transform{1.f};
        cc::mat4 m_node_transform{1.f};
        bool m_node_transform_dirty = true;
        bool m_transform_updated = true;

        node *m_parent = nullptr;
        std::vector<std::shared_ptr<cc::node>> m_children;

        cc::color4 m_color = {255, 255, 255, 255};

    protected:

        virtual void draw();

        virtual const cc::mat4 &get_node_transform();

        virtual void update_color();

        virtual void update_transform(const cc::mat4 &transform);

    };

}