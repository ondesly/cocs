//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <unordered_map>

namespace cc {

    class entity;

    class scheduler {
    public:

        ~scheduler();

    public:

        void tick();

        void schedule_update(cc::entity *const target, const std::function<void()> &func);

        void unschedule_update(cc::entity *const target);

        void unschedule_all();

    private:

        std::unordered_map<cc::entity *, std::function<void()>> m_targets;

    };

}