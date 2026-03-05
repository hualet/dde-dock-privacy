# SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

project(dock-privacyindicator)

FILE(GLOB_RECURSE SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.json"
)

find_package(Qt6 REQUIRED COMPONENTS LinguistTools DBus)
find_package(DDEShell REQUIRED)

add_library(dock-privacyindicator SHARED ${SRCS})

target_link_libraries(dock-privacyindicator PRIVATE
    Dde::Shell
    Qt6::DBus
)

ds_install_package(PACKAGE org.deepin.ds.dock.privacyindicator TARGET dock-privacyindicator)
ds_handle_package_translation(PACKAGE org.deepin.ds.dock.privacyindicator)
