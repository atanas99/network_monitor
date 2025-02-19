cmake_minimum_required(VERSION 3.27)
project(network_monitor)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Charts)

include_directories(include)

set(SOURCES
        src/main.cpp
        src/networkmonitor.cpp
        src/networktable.cpp
        src/networkchart.cpp
        src/dataupdater.cpp
        src/uicomponents.cpp
        src/axissetup.cpp
        src/chartsetup.cpp
)

set(HEADERS
        include/networkmonitor.h
        include/networktable.h
        include/networkchart.h
        include/dataupdater.h
        include/uicomponents.h
        include/axissetup.h
        include/chartsetup.h
)

add_executable(network_monitor ${SOURCES} ${HEADERS})

target_link_libraries(network_monitor Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Charts)

target_include_directories(network_monitor PRIVATE include)