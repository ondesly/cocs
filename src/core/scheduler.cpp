//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include "core/entity.h"

#include "scheduler.h"

cc::scheduler::~scheduler() {
    unschedule_all();
}

void cc::scheduler::schedule_update(entity *const target, const std::function<void()> &func) {
    m_targets[target] = func;
}

void cc::scheduler::unschedule_update(entity *const target) {
    m_targets.erase(target);
}

void cc::scheduler::unschedule_all() {
    m_targets.clear();
}

void cc::scheduler::tick() {
    for (auto &pair : m_targets) {
        if (pair.first->is_running()) {
            pair.second();
        }
    }
}