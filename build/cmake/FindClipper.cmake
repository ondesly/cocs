#
# Cocs Micro Engine
# Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
#

set(CLIPPER_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../libs/clipper)

set(CLIPPER_INCLUDE_DIRS ${CLIPPER_ROOT}/include)
set(CLIPPER_LIBRARIES ${CLIPPER_ROOT}/prebuilt/android/${ANDROID_ABI}/libclipper.a)