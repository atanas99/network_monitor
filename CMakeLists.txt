cmake_minimum_required(VERSION 3.27)
project(network_monitor)

set(CMAKE_CXX_STANDARD 23)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(network_monitor main.cpp networkmonitor.cpp
        networktable.cpp
        networktable.h
        networkchart.cpp
        networkchart.h
        dataupdater.cpp
        dataupdater.h
        uicomponents.cpp
        uicomponents.h
        axissetup.cpp
        axissetup.h
        chartsetup.cpp
        chartsetup.h)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Charts)
target_link_libraries(network_monitor Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Charts)



