//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <android/bitmap.h>
#include <jni.h>

#include "core/android/jvm.h"

#include "text_renderer_impl.h"

cc::text_renderer_impl::text_renderer_impl(ANativeActivity *const activity) {
    m_jvm = new jvm(activity);

    JNIEnv *env;
    m_jvm->attach_thread(&env);

    m_jvm->preload_class(env, "android/text/TextPaint");
    m_jvm->preload_class(env, "android/text/Layout$Alignment");
    m_jvm->preload_class(env, "android/text/StaticLayout");
    m_jvm->preload_class(env, "android/graphics/Bitmap$Config");
    m_jvm->preload_class(env, "android/graphics/Bitmap");
    m_jvm->preload_class(env, "android/graphics/Canvas");
    m_jvm->preload_class(env, "android/graphics/Color");

    m_jvm->preload_method_id(env, "TextPaint", "<init>", "()V");
    m_jvm->preload_method_id(env, "TextPaint", "setTextSize", "(F)V");
    m_jvm->preload_method_id(env, "TextPaint", "setColor", "(I)V");
    m_jvm->preload_method_id(env, "TextPaint", "setAntiAlias", "(Z)V");
    m_jvm->preload_method_id(env, "TextPaint", "measureText", "(Ljava/lang/String;)F");

    m_jvm->preload_method_id(env, "StaticLayout", "<init>",
                             "(Ljava/lang/CharSequence;Landroid/text/TextPaint;ILandroid/text/Layout$Alignment;FFZ)V");
    m_jvm->preload_method_id(env, "StaticLayout", "getWidth", "()I");
    m_jvm->preload_method_id(env, "StaticLayout", "getHeight", "()I");
    m_jvm->preload_method_id(env, "StaticLayout", "draw", "(Landroid/graphics/Canvas;)V");

    m_jvm->preload_method_id(env, "Canvas", "<init>", "()V");
    m_jvm->preload_method_id(env, "Canvas", "setBitmap", "(Landroid/graphics/Bitmap;)V");

    m_jvm->preload_method_id(env, "Bitmap", "createBitmap",
                             "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;", true);

    m_jvm->preload_field_id(env, "Layout$Alignment", "ALIGN_CENTER", "Landroid/text/Layout$Alignment;");
    m_jvm->preload_field_id(env, "Bitmap$Config", "ARGB_8888", "Landroid/graphics/Bitmap$Config;");
    m_jvm->preload_field_id(env, "Color", "WHITE", "I");

    m_jvm->preload_object(env, "TextPaint", "TextPaint::<init>");
    m_jvm->preload_object(env, "Canvas", "Canvas::<init>");

    m_jvm->preload_object_field(env, "Layout$Alignment", "Layout$Alignment::ALIGN_CENTER");
    m_jvm->preload_object_field(env, "Bitmap$Config", "Bitmap$Config::ARGB_8888");
    m_jvm->preload_int_field(env, "Color", "Color::WHITE");

    //

    auto text_paint = m_jvm->object("TextPaint");
    env->CallVoidMethod(text_paint, m_jvm->method("TextPaint::setColor"), m_jvm->int_field("Color"));
    env->CallVoidMethod(text_paint, m_jvm->method("TextPaint::setAntiAlias"), true);

    //

    m_jvm->detach_thread();
}

cc::text_renderer_impl::~text_renderer_impl() {
    delete m_jvm;
}

cc::float2 cc::text_renderer_impl::render(const std::string &value, const float font_size, float max_width, unsigned char *&data) {
    float2 size;

    JNIEnv *env;
    m_jvm->attach_thread(&env);

    if (!env) {
        data = nullptr;
        return {0.f, 0.f};
    }

    // String

    auto string = env->NewStringUTF(value.c_str());

    // TextPaint

    auto text_paint = m_jvm->object("TextPaint");

    if (!text_paint) {
        data = nullptr;
        return {0.f, 0.f};
    }

    env->CallVoidMethod(text_paint, m_jvm->method("TextPaint::setTextSize"), font_size);

    if (max_width == 0) {
        max_width = env->CallFloatMethod(text_paint, m_jvm->method("TextPaint::measureText"),
                                         string);
    }

    // StaticLayout

    auto static_layout = env->NewObject(m_jvm->clazz("StaticLayout"), m_jvm->method("StaticLayout::<init>"), string,
                                        text_paint, int(max_width), m_jvm->object("Layout$Alignment"), 1.f, 0.f, false);

    if (!static_layout) {
        data = nullptr;
        return {0.f, 0.f};
    }

    int width = env->CallIntMethod(static_layout, m_jvm->method("StaticLayout::getWidth"));
    int height = env->CallIntMethod(static_layout, m_jvm->method("StaticLayout::getHeight"));

    // Bitmap

    auto bitmap = env->CallStaticObjectMethod(m_jvm->clazz("Bitmap"), m_jvm->method("Bitmap::createBitmap"), width,
                                              height, m_jvm->object("Bitmap$Config"));

    if (!bitmap) {
        data = nullptr;
        return {0.f, 0.f};
    }

    // Canvas

    auto canvas = m_jvm->object("Canvas");

    if (!canvas) {
        data = nullptr;
        return {0.f, 0.f};
    }

    env->CallVoidMethod(canvas, m_jvm->method("Canvas::setBitmap"), bitmap);
    env->CallVoidMethod(static_layout, m_jvm->method("StaticLayout::draw"), canvas);

    //

    AndroidBitmapInfo info;
    void *pixels;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0) {
        data = nullptr;
        return {0.f, 0.f};
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0) {
        data = nullptr;
        return {0.f, 0.f};
    }

    //

    auto bitmap_data = (unsigned char *) pixels;

    auto len = info.width * info.height * 4;
    data = new unsigned char[len];
    std::copy(bitmap_data, bitmap_data + len, data);

    size = float2(info.width, info.height);

    //

    AndroidBitmap_unlockPixels(env, bitmap);

    //

    env->DeleteLocalRef(string);

    //

    m_jvm->detach_thread();

    return size;
}