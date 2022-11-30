cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(BPR_NAMI)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(OGRE REQUIRED CONFIG)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/hybrid-a-star/include/
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

    hybrid-a-star/src/algorithm.cpp
    hybrid-a-star/src/bucketedqueue.cpp
    hybrid-a-star/src/collisiondetection.cpp
    hybrid-a-star/src/dubins.cpp
    hybrid-a-star/src/dynamicvoronoi.cpp
    hybrid-a-star/src/hybridastar.cpp
    hybrid-a-star/src/node2d.cpp
    hybrid-a-star/src/node3d.cpp
    hybrid-a-star/src/path.cpp
    hybrid-a-star/src/smoother.cpp

    include/settings.h
    include/mytcpsocket.h
    include/barrier.h
    include/car.h
    include/gamemap.h
    include/locationmap.h
    include/myexception.h
    include/mygraph.h
    include/logger.h
    include/occurancygrid.h
)
find_package(ompl REQUIRED)
include_directories(include ${OMPL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OMPL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network OgreMain OgreBites)

configure_file(data/config.cfg ${CMAKE_BINARY_DIR}/config.cfg @ONLY)
