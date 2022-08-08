cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(BPR_NAMI)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(OGRE REQUIRED CONFIG)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
)

add_executable(${PROJECT_NAME}
    src/settings.cpp
    src/main.cpp
    src/mytcpsocket.cpp
    src/barrier.cpp
    src/car.cpp
    src/gamemap.cpp
    src/locationmap.cpp
    src/myexception.cpp
    src/mygraph.cpp
    src/logger.cpp

    include/settings.h
    include/mytcpsocket.h
    include/barrier.h
    include/car.h
    include/gamemap.h
    include/locationmap.h
    include/myexception.h
    include/mygraph.h
    include/logger.h
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network OgreMain OgreBites)

configure_file(data/config.cfg ${CMAKE_BINARY_DIR}/config.cfg @ONLY)
