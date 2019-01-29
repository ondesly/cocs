#
# Cocs Micro Engine
# Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
#

set(CAIRO_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../libs/cairo)

set(CAIRO_INCLUDE_DIRS ${CAIRO_ROOT}/include)
set(CAIRO_LIBRARIES ${CAIRO_ROOT}/prebuilt/android/${ANDROID_ABI}/libcairo.a)