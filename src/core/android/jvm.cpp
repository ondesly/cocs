//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <android_native_app_glue.h>
#include <jni.h>

#include "jvm.hpp"

cc::clazz::clazz(const std::string &name) : m_name(name) {
    auto env = cc::jvm::i()->get_env();
    m_class = (jclass) env->NewGlobalRef(env->FindClass(name.c_str()));
    if (env->ExceptionCheck()) throw std::logic_error("jni");

}

cc::clazz::~clazz() {
    auto env = cc::jvm::i()->get_env();
    env->DeleteGlobalRef(m_class);
}

jclass cc::clazz::get_clazz() {
    return m_class;
}

//

cc::static_method::static_method(cc::clazz *cl, const std::string &name, const std::string &signature)
        : m_class(cl) {
    auto env = cc::jvm::i()->get_env();
    m_method_id = env->GetStaticMethodID(cl->get_clazz(), name.c_str(), signature.c_str());
    if (env->ExceptionCheck()) throw std::logic_error("jni");
}

jobject cc::static_method::call(int n, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, n);

    auto object = env->CallStaticObjectMethodV(m_class->get_clazz(), m_method_id, args);
    if (env->ExceptionCheck()) throw std::logic_error("jni");

    va_end(args);

    return object;
}

//

cc::method::method(cc::clazz *cl, const std::string &name, const std::string &signature) {
    auto env = cc::jvm::i()->get_env();
    m_method_id = env->GetMethodID(cl->get_clazz(), name.c_str(), signature.c_str());
    if (env->ExceptionCheck()) throw std::logic_error("jni");
}

void cc::method::call(jobject obj, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, obj);

    env->CallVoidMethodV(obj, m_method_id, args);
    if (env->ExceptionCheck()) throw std::logic_error("jni");

    va_end(args);
}

int cc::method::call_int(jobject obj, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, obj);

    auto value = env->CallIntMethodV(obj, m_method_id, args);
    if (env->ExceptionCheck()) throw std::logic_error("jni");

    va_end(args);

    return value;
}

float cc::method::call_float(jobject obj, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, obj);

    auto value = env->CallFloatMethodV(obj, m_method_id, args);
    if (env->ExceptionCheck()) throw std::logic_error("jni");

    va_end(args);

    return value;
}

jobject cc::method::call_object(jobject obj, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, obj);

    auto object = env->CallObjectMethodV(obj, m_method_id, args);
    if (env->ExceptionCheck()) throw std::logic_error("jni");

    va_end(args);

    return object;
}

//

cc::constructor::constructor(cc::clazz *cl, const std::string &signature) : m_class(cl) {
    auto env = cc::jvm::i()->get_env();
    m_method_id = env->GetMethodID(cl->get_clazz(), "<init>", signature.c_str());
    if (env->ExceptionCheck()) throw std::logic_error("jni");
}

jobject cc::constructor::call(int n, ...) {
    auto env = cc::jvm::i()->get_env();

    va_list args;
    va_start(args, n);

    auto object = env->NewObjectV(m_class->get_clazz(), m_method_id, args);

    va_end(args);

    return object;
}

//

cc::static_field::static_field(cc::clazz *cl, const std::string &name, const std::string &signature)
        : m_class(cl) {
    auto env = cc::jvm::i()->get_env();
    m_field_id = env->GetStaticFieldID(cl->get_clazz(), name.c_str(), signature.c_str());
    if (env->ExceptionCheck()) throw std::logic_error("jni");
}

int cc::static_field::get_int() {
    auto env = cc::jvm::i()->get_env();
    auto value = env->GetStaticIntField(m_class->get_clazz(), m_field_id);
    if (env->ExceptionCheck()) throw std::logic_error("jni");
    return value;
}

jobject cc::static_field::get_object() {
    auto env = cc::jvm::i()->get_env();
    auto value = env->GetStaticObjectField(m_class->get_clazz(), m_field_id);
    if (env->ExceptionCheck()) throw std::logic_error("jni");
    return value;
}

//

cc::jvm::~jvm() {
    m_activity->vm->DetachCurrentThread();
}

void cc::jvm::set_activity(ANativeActivity *const activity) {
    m_activity = activity;
}

ANativeActivity *cc::jvm::get_activity() const {
    return m_activity;
}

JNIEnv *cc::jvm::get_env() {
    auto it = m_envs.find(std::this_thread::get_id());
    if (it == m_envs.end()) {
        JNIEnv *env = nullptr;
        m_activity->vm->AttachCurrentThread(&env, nullptr);
        m_envs[std::this_thread::get_id()] = env;
        return env;
    } else {
        return (*it).second;
    }
}

jobject cc::jvm::new_global_ref(jobject obj) {
    auto env = cc::jvm::i()->get_env();
    return env->NewGlobalRef(obj);
}

void cc::jvm::delete_global_ref(jobject obj) {
    auto env = cc::jvm::i()->get_env();
    env->DeleteGlobalRef(obj);
}

jstring cc::jvm::new_string(const std::string &s) {
    auto env = cc::jvm::i()->get_env();
    return env->NewStringUTF(s.c_str());
}

void cc::jvm::delete_local_ref(jobject obj) {
    auto env = cc::jvm::i()->get_env();
    return env->DeleteLocalRef(obj);
}