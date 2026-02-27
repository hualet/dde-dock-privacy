cmake_minimum_required(VERSION 3.16)

set(PLUGIN_NAME "privacy-indicator")

project(${PLUGIN_NAME})

# 使用 Qt6（系统已安装）

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 明确列出我们的源文件，不要使用 GLOB_RECURSE
set(SRCS
    privacyindicatorplugin.cpp
    devicemonitor.cpp
    privacywidget.cpp
)

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg DBus)
find_package(Dtk6Widget REQUIRED)

add_definitions("-DQT_NO_DEBUG_OUTPUT")
add_library(${PLUGIN_NAME} SHARED ${SRCS} privacy-indicator.qrc)

set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ./)

target_include_directories(${PLUGIN_NAME} PUBLIC
    ${Dtk6Widget_INCLUDE_DIRS}
    /usr/include/dde-dock
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    ${Dtk6Widget_LIBRARIES}
    Qt6::Widgets
    Qt6::Svg
    Qt6::DBus
)

set(CMAKE_INSTALL_PREFIX "/usr")

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins)
install(FILES privacy-indicator.json DESTINATION share/dde-dock/plugins/)
install(DIRECTORY resources/ DESTINATION share/dde-dock/plugins/privacy-indicator/resources)
