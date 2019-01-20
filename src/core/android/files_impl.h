//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>

#include "core/files.h"

struct AAssetManager;

namespace cc {

    class files_impl : public files {
    public:

        files_impl(AAssetManager *const asset_manager);

    public:

        std::string get_content(const std::string &path) override;

    private:

        AAssetManager *m_asset_manager;

    };

}