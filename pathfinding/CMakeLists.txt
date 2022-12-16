cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(pathfinding)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ompl REQUIRED)

include_directories(
  ${OMPL_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}    
    src/algorithm.cpp
    src/bucketedqueue.cpp
    src/collisiondetection.cpp
    src/dubins.cpp
    src/dynamicvoronoi.cpp
    src/hybridastar.cpp
    src/node2d.cpp
    src/node3d.cpp
    src/smoother.cpp
)

add_library(sub::pathfinding ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} ${OMPL_LIBRARIES})