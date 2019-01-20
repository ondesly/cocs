//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "files_impl.h"

cc::files_impl::files_impl(AAssetManager *const asset_manager) : m_asset_manager(asset_manager) {

}

std::string cc::files_impl::get_content(const std::string &path) {
    auto asset = AAssetManager_open(m_asset_manager, path.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        return "";
    }

    auto size = size_t(AAsset_getLength(asset));
    char buffer[size];
    AAsset_read(asset, buffer, size);
    AAsset_close(asset);

    return std::string(buffer, size);
}