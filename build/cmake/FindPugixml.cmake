#
# Cocs Micro Engine
# Copyright (C) 2018-2019 Dmitriy Torkhov <dmitriytorkhov@gmail.com>
#

set(PUGIXML_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../libs/pugixml)

set(PUGIXML_INCLUDE_DIRS ${PUGIXML_ROOT}/include)
set(PUGIXML_LIBRARIES ${PUGIXML_ROOT}/prebuilt/android/${ANDROID_ABI}/libpugixml.a)