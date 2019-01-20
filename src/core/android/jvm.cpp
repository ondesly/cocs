//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <android_native_app_glue.h>
#include <jni.h>

#include "jvm.h"

cc::jvm::jvm(ANativeActivity *const activity) : m_activity(activity) {

}

cc::jvm::~jvm() {
    JNIEnv *env;
    m_activity->vm->AttachCurrentThread(&env, nullptr);

    for (auto pair : m_classes) {
        env->DeleteGlobalRef(pair.second);
    }
    for (auto pair : m_objects) {
        env->DeleteGlobalRef(pair.second);
    }

    m_activity->vm->DetachCurrentThread();
}

void cc::jvm::attach_thread(JNIEnv **env) {
    m_activity->vm->AttachCurrentThread(env, nullptr);
}

void cc::jvm::detach_thread() {
    m_activity->vm->DetachCurrentThread();
}

jclass cc::jvm::clazz(const std::string &name) {
    return m_classes[name];
}

jmethodID cc::jvm::method(const std::string &name) {
    return m_method_ids[name];
}

jfieldID cc::jvm::field(const std::string &name) {
    return m_field_ids[name];
}

int cc::jvm::int_field(const std::string &name) {
    return m_int_fields[name];
}

jobject cc::jvm::object(const std::string &name) {
    return m_objects[name];
}

bool cc::jvm::preload_class(JNIEnv *env, const std::string &class_name) {
    auto object_class = env->FindClass(class_name.c_str());
    if (object_class) {
        auto name = class_name.substr(class_name.rfind("/") + 1);
        m_classes[name] = static_cast<jclass>(env->NewGlobalRef(object_class));
        return true;
    } else {
        return false;
    }
}

bool cc::jvm::preload_method_id(JNIEnv *env, const std::string &class_name, const char *method_name,
                                const char *sig, const bool is_static/* = false*/) {
    jmethodID method_id;
    if (is_static) {
        method_id = env->GetStaticMethodID(m_classes[class_name], method_name, sig);
    } else {
        method_id = env->GetMethodID(m_classes[class_name], method_name, sig);
    }

    if (method_id) {
        m_method_ids[class_name + "::" + method_name] = method_id;
        return true;
    } else {

        return false;
    }
}

bool cc::jvm::preload_field_id(JNIEnv *env, const std::string &class_name, const char *field_name,
                               const char *sig) {
    auto field_id = env->GetStaticFieldID(m_classes[class_name], field_name, sig);
    if (field_id) {
        m_field_ids[class_name + "::" + field_name] = field_id;
        return true;
    } else {
        return false;
    }
}

bool cc::jvm::preload_object(JNIEnv *env, const char *class_name, const char *constructor_name) {
    auto object = env->NewObject(m_classes[class_name], m_method_ids[constructor_name]);
    if (object) {
        m_objects[class_name] = env->NewGlobalRef(object);
        return true;
    } else {
        return false;
    }
}

bool cc::jvm::preload_object_field(JNIEnv *env, const char *class_name, const char *field_name) {
    auto object = env->GetStaticObjectField(m_classes[class_name], m_field_ids[field_name]);
    if (object) {
        m_objects[class_name] = env->NewGlobalRef(object);
        return true;
    } else {
        return false;
    }
}

bool cc::jvm::preload_int_field(JNIEnv *env, const char *class_name, const char *field_name) {
    auto value = env->GetStaticIntField(m_classes[class_name], m_field_ids[field_name]);
    m_int_fields[class_name] = value;
    return true;
}
