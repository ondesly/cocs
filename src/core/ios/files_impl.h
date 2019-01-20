//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include "core/files.h"

namespace cc {

    class files_impl : public files {
    public:

        std::string get_content(const std::string &path) override;

    };

}