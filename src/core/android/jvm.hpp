//
// Cocs Micro Engine
// Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include "core/singleton.hpp"

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

    class clazz {
    public:

        explicit clazz(const std::string &name);

        ~clazz();

        jclass get_clazz();

    private:

        const std::string m_name;
        jclass m_class;

    };

    //

    class static_method {
    public:

        static_method(clazz *cl, const std::string &name, const std::string &signature);

        jobject call(int n = 0, ...);

    private:

        jmethodID m_method_id;
        cc::clazz *m_class;

    };

    //

    class method {
    public:

        method(clazz *cl, const std::string &name, const std::string &signature);

        void call(jobject obj, ...);

        int call_int(jobject obj, ...);

        float call_float(jobject obj, ...);

        jobject call_object(jobject obj, ...);

    private:

        jmethodID m_method_id;

    };

    //

    class constructor {
    public:

        constructor(clazz *cl, const std::string &signature);

        jobject call(int n = 0, ...);

    private:

        jmethodID m_method_id;
        cc::clazz *m_class;

    };

    //

    class static_field {
    public:

        static_field(clazz *cl, const std::string &name, const std::string &signature);

        int get_int();

        jobject get_object();

    private:

        jfieldID m_field_id;
        cc::clazz *m_class;

    };

}

namespace cc {

    class jvm : public singleton<jvm> {
        friend class singleton<jvm>;

    public:

        ~jvm();

    public:

        void set_activity(ANativeActivity *activity);

        ANativeActivity *get_activity() const;

        JNIEnv *get_env();

        jobject new_global_ref(jobject obj);

        void delete_global_ref(jobject obj);

        jstring new_string(const std::string &s);

        void delete_local_ref(jobject obj);

    private:

        ANativeActivity *m_activity;

        std::unordered_map<std::thread::id, JNIEnv *> m_envs;

    };

}