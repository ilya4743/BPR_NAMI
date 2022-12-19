cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(pathfinding)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  include ${OMPL_INCLUDE_DIRS}
)

find_package(ompl REQUIRED)

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
  src/occupancygrid.cpp
)

target_link_libraries(${PROJECT_NAME} ${OMPL_LIBRARIES})

add_library(sub::pathfinding ALIAS ${PROJECT_NAME})