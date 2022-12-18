cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(BPR_NAMI)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/pathfinding/include/
    include ${OMPL_INCLUDE_DIRS}
)

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(OGRE REQUIRED CONFIG)

add_subdirectory(pathfinding)

add_executable(${PROJECT_NAME}
    src/settings.cpp
    src/main.cpp
    src/mytcpsocket.cpp
    src/barrier.cpp
    src/car.cpp
    src/gamemap.cpp
    src/myexception.cpp
    src/logger.cpp
    src/DataUnpacker.cpp

    include/settings.h
    include/mytcpsocket.h
)

target_link_libraries(${PROJECT_NAME} Qt5::Network OgreMain sub::pathfinding ${OMPL_LIBRARIES})

configure_file(data/config.cfg ${CMAKE_BINARY_DIR}/config.cfg @ONLY)