//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <unordered_map>
#include <vector>

#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>

#include "jvm.hpp"

#include "../device.hpp"

namespace {

    struct {
        cc::clazz *clazz;
        cc::static_method *fromFd;
    } ParcelFileDescriptor;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *getFileDescriptor;
    } AssetFileDescriptor;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *setDataSource;
        cc::method *prepare;
        cc::method *start;
        cc::method *pause;
        cc::method *stop;
        cc::method *release;
    } MediaPlayer;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *setTextSize;
        cc::method *setColor;
        cc::method *setAntiAlias;
        cc::method *measureText;
    } TextPaint;

    struct {
        cc::clazz *clazz;
        cc::static_field *ALIGN_CENTER;
    } Layout$Alignment;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *getWidth;
        cc::method *getHeight;
        cc::method *draw;
    } StaticLayout;

    struct {
        cc::clazz *clazz;
        cc::static_method *createBitmap;
    } Bitmap;

    struct {
        cc::clazz *clazz;
        cc::static_field *ARGB_8888;
    } Bitmap$Config;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *setBitmap;
    } Canvas;

    struct {
        cc::clazz *clazz;
        cc::static_field *WHITE;
    } Color;

    struct {
        cc::clazz *clazz;
        cc::static_field *STREAM_MUSIC;
    } AudioManager;

    struct {
        cc::clazz *clazz;
        cc::constructor *constructor;
        cc::method *load;
        cc::method *play;
        cc::method *release;
        cc::method *setVolume;
    } SoundPool;

    //

    jobject m_media_player = nullptr;

    int m_stream_music;
    jobject m_sound_pool = nullptr;
    std::unordered_map<std::string, int> m_sound_ids;

    jobject m_text_paint;
    jobject m_layout_alignment;
    jobject m_bitmap_config;
    jobject m_canvas;

}

cc::device::device() {

    // Assets

    ParcelFileDescriptor.clazz = new clazz("android/os/ParcelFileDescriptor");
    ParcelFileDescriptor.fromFd = new static_method(ParcelFileDescriptor.clazz, "fromFd", "(I)Landroid/os/ParcelFileDescriptor;");

    AssetFileDescriptor.clazz = new clazz("android/content/res/AssetFileDescriptor");
    AssetFileDescriptor.constructor = new constructor(AssetFileDescriptor.clazz, "(Landroid/os/ParcelFileDescriptor;JJ)V");
    AssetFileDescriptor.getFileDescriptor = new method(AssetFileDescriptor.clazz, "getFileDescriptor", "()Ljava/io/FileDescriptor;");

    // Music

    MediaPlayer.clazz = new clazz("android/media/MediaPlayer");
    MediaPlayer.constructor = new constructor(MediaPlayer.clazz, "()V");
    MediaPlayer.setDataSource = new method(MediaPlayer.clazz, "setDataSource", "(Ljava/io/FileDescriptor;JJ)V");
    MediaPlayer.prepare = new method(MediaPlayer.clazz, "prepare", "()V");
    MediaPlayer.start = new method(MediaPlayer.clazz, "start", "()V");
    MediaPlayer.pause = new method(MediaPlayer.clazz, "pause", "()V");
    MediaPlayer.stop = new method(MediaPlayer.clazz, "stop", "()V");
    MediaPlayer.release = new method(MediaPlayer.clazz, "release", "()V");

    // Sounds

    AudioManager.clazz = new clazz("android/media/AudioManager");
    AudioManager.STREAM_MUSIC = new static_field(AudioManager.clazz, "STREAM_MUSIC", "I");

    SoundPool.clazz = new clazz("android/media/SoundPool");
    SoundPool.constructor = new constructor(SoundPool.clazz, "(III)V");
    SoundPool.load = new method(SoundPool.clazz, "load", "(Ljava/io/FileDescriptor;JJI)I");
    SoundPool.play = new method(SoundPool.clazz, "play", "(IFFIIF)I");
    SoundPool.release = new method(SoundPool.clazz, "release", "()V");
    SoundPool.setVolume = new method(SoundPool.clazz, "setVolume", "(IFF)V");

    //

    m_stream_music = AudioManager.STREAM_MUSIC->get_int();

    // Text

    TextPaint.clazz = new clazz("android/text/TextPaint");
    TextPaint.constructor = new constructor(TextPaint.clazz, "()V");
    TextPaint.setTextSize = new method(TextPaint.clazz, "setTextSize", "(F)V");
    TextPaint.setColor = new method(TextPaint.clazz, "setColor", "(I)V");
    TextPaint.setAntiAlias = new method(TextPaint.clazz, "setAntiAlias", "(Z)V");
    TextPaint.measureText = new method(TextPaint.clazz, "measureText", "(Ljava/lang/String;)F");

    Color.clazz = new clazz("android/graphics/Color");
    Color.WHITE = new static_field(Color.clazz, "WHITE", "I");

    Layout$Alignment.clazz = new clazz("android/text/Layout$Alignment");
    Layout$Alignment.ALIGN_CENTER = new static_field(Layout$Alignment.clazz, "ALIGN_CENTER", "Landroid/text/Layout$Alignment;");

    StaticLayout.clazz = new clazz("android/text/StaticLayout");
    StaticLayout.constructor = new constructor(StaticLayout.clazz, "(Ljava/lang/CharSequence;Landroid/text/TextPaint;ILandroid/text/Layout$Alignment;FFZ)V");
    StaticLayout.getWidth = new method(StaticLayout.clazz, "getWidth", "()I");
    StaticLayout.getHeight = new method(StaticLayout.clazz, "getHeight", "()I");
    StaticLayout.draw = new method(StaticLayout.clazz, "draw", "(Landroid/graphics/Canvas;)V");

    Bitmap$Config.clazz = new clazz("android/graphics/Bitmap$Config");
    Bitmap$Config.ARGB_8888 = new static_field(Bitmap$Config.clazz, "ARGB_8888", "Landroid/graphics/Bitmap$Config;");

    Bitmap.clazz = new clazz("android/graphics/Bitmap");
    Bitmap.createBitmap = new static_method(Bitmap.clazz, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    Canvas.clazz = new clazz("android/graphics/Canvas");
    Canvas.constructor = new constructor(Canvas.clazz, "()V");
    Canvas.setBitmap = new method(Canvas.clazz, "setBitmap", "(Landroid/graphics/Bitmap;)V");

    //

    m_text_paint = jvm::i()->new_global_ref(TextPaint.constructor->call());

    TextPaint.setColor->call(m_text_paint, Color.WHITE->get_int());
    TextPaint.setAntiAlias->call(m_text_paint, true);

    m_layout_alignment = jvm::i()->new_global_ref(Layout$Alignment.ALIGN_CENTER->get_object());
    m_bitmap_config = jvm::i()->new_global_ref(Bitmap$Config.ARGB_8888->get_object());

    m_canvas = jvm::i()->new_global_ref(Canvas.constructor->call());
}

cc::device::~device() {
    clear_music();
    clear_sounds();

    jvm::i()->delete_global_ref(m_text_paint);
    jvm::i()->delete_global_ref(m_layout_alignment);
    jvm::i()->delete_global_ref(m_bitmap_config);
    jvm::i()->delete_global_ref(m_canvas);
}

void cc::device::load_music(const std::string &path) {
    clear_music();

    m_media_player = jvm::i()->new_global_ref(MediaPlayer.constructor->call());

    //

    auto asset = AAssetManager_open(jvm::i()->get_activity()->assetManager, (path + ".mp3").c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        return;
    }

    off_t offset, length;
    int fd = AAsset_openFileDescriptor(asset, &offset, &length);

    //

    auto parcel_file_descriptor = ParcelFileDescriptor.fromFd->call(0, fd);
    auto asset_file_descriptor = AssetFileDescriptor.constructor->call(0, parcel_file_descriptor, offset, length);
    auto file_descriptor = AssetFileDescriptor.getFileDescriptor->call_object(asset_file_descriptor);

    MediaPlayer.setDataSource->call(m_media_player, file_descriptor, offset, length);
    MediaPlayer.prepare->call(m_media_player);

    //

    AAsset_close(asset);
}

void cc::device::play_music() {
    if (m_media_player) {
        MediaPlayer.start->call(m_media_player);
    }
}

void cc::device::pause_music() {
    if (m_media_player) {
        MediaPlayer.pause->call(m_media_player);
    }
}

void cc::device::stop_music() {
    if (m_media_player) {
        MediaPlayer.stop->call(m_media_player);
    }
}

void cc::device::clear_music() {
    if (m_media_player) {
        MediaPlayer.stop->call(m_media_player);
        MediaPlayer.release->call(m_media_player);

        jvm::i()->delete_global_ref(m_media_player);

        m_media_player = nullptr;
    }
}

void cc::device::load_sounds(const std::vector<std::string> &paths) {
    if (m_sound_pool) {
        SoundPool.release->call(m_sound_pool);
        m_sound_ids.clear();
    }

    m_sound_pool = jvm::i()->new_global_ref(SoundPool.constructor->call(3, m_stream_music, 5));

    for (auto &path : paths) {
        auto asset = AAssetManager_open(jvm::i()->get_activity()->assetManager, (path + ".mp3").c_str(), AASSET_MODE_UNKNOWN);
        if (!asset) {
            return;
        }

        off_t offset, length;
        int fd = AAsset_openFileDescriptor(asset, &offset, &length);

        //

        auto parcel_file_descriptor = ParcelFileDescriptor.fromFd->call(0, fd);
        auto asset_file_descriptor = AssetFileDescriptor.constructor->call(0, parcel_file_descriptor, offset, length);
        auto file_descriptor = AssetFileDescriptor.getFileDescriptor->call_object(asset_file_descriptor);

        m_sound_ids[path] = SoundPool.load->call_int(m_sound_pool, file_descriptor, offset, length, 0);

        //

        AAsset_close(asset);
    }
}

void cc::device::play_sound(const std::string &path) {
    auto it = m_sound_ids.find(path);
    if (it != m_sound_ids.end()) {
        int stream_id = SoundPool.play->call_int(m_sound_pool, (*it).second, 1.f, 1.f, 1, 0, 1.f);
        SoundPool.setVolume->call(m_sound_pool, stream_id, 1.f, 1.f);
    }
}

void cc::device::clear_sounds() {
    if (m_sound_pool) {
        SoundPool.release->call(m_sound_pool);
        jvm::i()->delete_global_ref(m_sound_pool);

        m_sound_pool = nullptr;
        m_sound_ids.clear();
    }
}

std::string cc::device::get_file_content(const std::string &path) {
    auto asset = AAssetManager_open(jvm::i()->get_activity()->assetManager, path.c_str(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        return "";
    }

    auto size = size_t(AAsset_getLength(asset));
    char buffer[size];
    AAsset_read(asset, buffer, size);
    AAsset_close(asset);

    return std::string(buffer, size);
}

cc::float2 cc::device::render_text(const std::string &value, const float font_size, float max_width, unsigned char *&data) {

    // TextPaint

    TextPaint.setTextSize->call(m_text_paint, font_size);

    auto string = jvm::i()->new_string(value);

    if (max_width == 0) {
        max_width = TextPaint.measureText->call_float(m_text_paint, string);
    }

    // StaticLayout

    auto static_layout = StaticLayout.constructor->call(0, string, m_text_paint, int(max_width), m_layout_alignment, 1.f, 0.f, false);

    int width = StaticLayout.getWidth->call_int(static_layout);
    int height = StaticLayout.getHeight->call_int(static_layout);

    // Draw

    auto bitmap = Bitmap.createBitmap->call(0, width, height, m_bitmap_config);
    Canvas.setBitmap->call(m_canvas, bitmap);
    StaticLayout.draw->call(static_layout, m_canvas);

    jvm::i()->delete_local_ref(string);

    //

    AndroidBitmapInfo info;
    void *pixels;

    if (AndroidBitmap_getInfo(jvm::i()->get_env(), bitmap, &info) < 0) {
        return {0.f, 0.f};
    }

    if (AndroidBitmap_lockPixels(jvm::i()->get_env(), bitmap, &pixels) < 0) {
        return {0.f, 0.f};
    }

    //

    auto bitmap_data = (unsigned char *) pixels;

    auto len = info.width * info.height * 4;
    data = new unsigned char[len];
    std::copy(bitmap_data, bitmap_data + len, data);

    //

    AndroidBitmap_unlockPixels(jvm::i()->get_env(), bitmap);

    //

    return float2(info.width, info.height);;
}