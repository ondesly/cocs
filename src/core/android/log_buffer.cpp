//
// Cocs Micro Engine
// Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
//

#include <streambuf>

#include <android/log.h>

#include "log_buffer.h"

const char *cc::log_buffer::TAG = "COCS";

cc::log_buffer::log_buffer() {
    setp(buffer, buffer + BUFFER_SIZE - 1);
}

int cc::log_buffer::overflow(const int c) {
    if (c == traits_type::eof()) {
        *pptr() = traits_type::to_char_type(c);
        sbumpc();
    }
    return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
}

int cc::log_buffer::sync() {
    int rc = 0;
    if (pbase() != pptr()) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "%s",
                            std::string(pbase(), pptr() - pbase()).c_str());
        rc = 0;
        setp(buffer, buffer + BUFFER_SIZE - 1);
    }
    return rc;
}
