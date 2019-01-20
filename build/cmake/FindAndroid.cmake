#
# Cocs Micro Engine
# Copyright (C) 2018 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
#

# build native_app_glue as a static lib
add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

#

set(ANDROID_INCLUDE_DIRS ${ANDROID_NDK}/sources/android/native_app_glue)

set(ANDROID_LIBRARIES
        android
        jnigraphics
        native_app_glue
        EGL
        GLESv2
        log)