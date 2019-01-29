#
# Cocs Micro Engine
# Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
#

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)

find_package(Android)
find_package(Cairo)
find_package(Clipper)
find_package(Pugixml)
find_package(Stb)

#

set(COCS_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR}/../src)

#

include_directories(
        ${ANDROID_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        ${CLIPPER_INCLUDE_DIRS}
        ${PUGIXML_INCLUDE_DIRS}
        ${STB_INCLUDE_DIRS}

        ${COCS_SRC_ROOT})

#

set(COCS_SRC
        ${COCS_SRC_ROOT}/core/android/activity.cpp
        ${COCS_SRC_ROOT}/core/android/device.cpp
        ${COCS_SRC_ROOT}/core/android/log_buffer.cpp
        ${COCS_SRC_ROOT}/core/android/jvm.cpp

        ${COCS_SRC_ROOT}/core/engine.cpp
        ${COCS_SRC_ROOT}/core/events.cpp
        ${COCS_SRC_ROOT}/core/value.cpp

        ${COCS_SRC_ROOT}/nodes/node.cpp
        ${COCS_SRC_ROOT}/nodes/sprite.cpp

        ${COCS_SRC_ROOT}/renderer/program.cpp
        ${COCS_SRC_ROOT}/renderer/render_area.cpp
        ${COCS_SRC_ROOT}/renderer/renderer.cpp

        ${COCS_SRC_ROOT}/textures/empty_frame.cpp
        ${COCS_SRC_ROOT}/textures/frame.cpp
        ${COCS_SRC_ROOT}/textures/image_renderer.cpp
        ${COCS_SRC_ROOT}/textures/svg_frame.cpp
        ${COCS_SRC_ROOT}/textures/text_frame.cpp
        ${COCS_SRC_ROOT}/textures/texture.cpp)

#

add_library(cocs STATIC ${COCS_SRC})

target_link_libraries(cocs
        ${ANDROID_LIBRARIES}
        ${CAIRO_LIBRARIES}
        ${CLIPPER_LIBRARIES}
        ${PUGIXML_LIBRARIES})