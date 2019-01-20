//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>
#include <unordered_map>

class ANativeActivity;

struct _JNIEnv;
typedef _JNIEnv JNIEnv;

class _jobject;

typedef _jobject *jobject;

struct _jmethodID;
typedef struct _jmethodID *jmethodID;

class _jclass;

typedef _jclass *jclass;

struct _jfieldID;
typedef struct _jfieldID *jfieldID;

namespace cc {

    class jvm {
    public:

        jvm(ANativeActivity *const activity);

        ~jvm();

    public:

        void attach_thread(JNIEnv **env);

        void detach_thread();

        bool preload_class(JNIEnv *env, const std::string &class_name);

        bool preload_method_id(JNIEnv *env, const std::string &class_name, const char *method_name, const char *sig, const bool is_static = false);

        bool preload_field_id(JNIEnv *env, const std::string &class_name, const char *field_name, const char *sig);

        bool preload_object(JNIEnv *env, const char *class_name, const char *constructor_name);

        bool preload_object_field(JNIEnv *env, const char *class_name, const char *field_name);

        bool preload_int_field(JNIEnv *env, const char *class_name, const char *field_name);

        jclass clazz(const std::string &name);

        jmethodID method(const std::string &name);

        jfieldID field(const std::string &name);

        int int_field(const std::string &name);

        jobject object(const std::string &name);

    private:

        ANativeActivity *m_activity;

        std::unordered_map<std::string, jclass> m_classes;
        std::unordered_map<std::string, jmethodID> m_method_ids;
        std::unordered_map<std::string, jfieldID> m_field_ids;
        std::unordered_map<std::string, jobject> m_objects;
        std::unordered_map<std::string, int> m_int_fields;

    };

}