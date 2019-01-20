//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>

#include "jvm.h"

#include "audio_impl.h"

cc::audio_impl::audio_impl(ANativeActivity *const activity)
        : m_asset_manager(activity->assetManager) {
    m_jvm = new jvm(activity);

    JNIEnv *env;
    m_jvm->attach_thread(&env);

    m_jvm->preload_class(env, "android/os/ParcelFileDescriptor");
    m_jvm->preload_class(env, "android/content/res/AssetFileDescriptor");
    m_jvm->preload_class(env, "android/media/MediaPlayer");

    m_jvm->preload_method_id(env, "ParcelFileDescriptor", "fromFd", "(I)Landroid/os/ParcelFileDescriptor;", true);
    m_jvm->preload_method_id(env, "AssetFileDescriptor", "<init>", "(Landroid/os/ParcelFileDescriptor;JJ)V");
    m_jvm->preload_method_id(env, "AssetFileDescriptor", "getFileDescriptor", "()Ljava/io/FileDescriptor;");
    m_jvm->preload_method_id(env, "MediaPlayer", "<init>", "()V");
    m_jvm->preload_method_id(env, "MediaPlayer", "setDataSource", "(Ljava/io/FileDescriptor;JJ)V");
    m_jvm->preload_method_id(env, "MediaPlayer", "prepare", "()V");
    m_jvm->preload_method_id(env, "MediaPlayer", "start", "()V");
    m_jvm->preload_method_id(env, "MediaPlayer", "stop", "()V");
    m_jvm->preload_method_id(env, "MediaPlayer", "release", "()V");

    m_jvm->detach_thread();
}

cc::audio_impl::~audio_impl() {
    JNIEnv *env;
    m_jvm->attach_thread(&env);

    for (auto &pair : m_players) {
        env->CallVoidMethod(pair.second, m_jvm->method("MediaPlayer::stop"));
        env->CallVoidMethod(pair.second, m_jvm->method("MediaPlayer::release"));
        env->DeleteGlobalRef(pair.second);
    }

    m_jvm->detach_thread();

    delete m_jvm;
}

void cc::audio_impl::preload(const std::vector<std::string> &paths) {
    JNIEnv *env;
    m_jvm->attach_thread(&env);

    for (auto &path : paths) {
        auto asset = AAssetManager_open(m_asset_manager, (path +
                                                          ".mp3").c_str(), AASSET_MODE_UNKNOWN);
        if (!asset) {
            return;
        }

        off_t offset, length;
        int fd = AAsset_openFileDescriptor(asset, &offset, &length);

        //

        auto parcel_file_descriptor = env->CallStaticObjectMethod(m_jvm->clazz("ParcelFileDescriptor"), m_jvm->method("ParcelFileDescriptor::fromFd"), fd);
        auto asset_file_descriptor = env->NewObject(m_jvm->clazz("AssetFileDescriptor"), m_jvm->method("AssetFileDescriptor::<init>"), parcel_file_descriptor, offset, length);

        auto file_descriptor = env->CallObjectMethod(asset_file_descriptor, m_jvm->method("AssetFileDescriptor::getFileDescriptor"));

        auto media_player = env->NewObject(m_jvm->clazz("MediaPlayer"), m_jvm->method("MediaPlayer::<init>"));

        env->CallVoidMethod(media_player, m_jvm->method("MediaPlayer::setDataSource"), file_descriptor, offset, length);
        env->CallVoidMethod(media_player, m_jvm->method("MediaPlayer::prepare"));

        m_players[path] = env->NewGlobalRef(media_player);

        //

        AAsset_close(asset);
    }

    m_jvm->detach_thread();
}

void cc::audio_impl::play(const std::string &path) {
    auto it = m_players.find(path);
    if (it != m_players.end()) {
        JNIEnv *env;
        m_jvm->attach_thread(&env);

        env->CallVoidMethod((*it).second, m_jvm->method("MediaPlayer::start"));

        m_jvm->detach_thread();
    }
}

void cc::audio_impl::stop(const std::string &path) {
    auto it = m_players.find(path);
    if (it != m_players.end()) {
        JNIEnv *env;
        m_jvm->attach_thread(&env);

        env->CallVoidMethod((*it).second, m_jvm->method("MediaPlayer::stop"));

        m_jvm->detach_thread();
    }
}