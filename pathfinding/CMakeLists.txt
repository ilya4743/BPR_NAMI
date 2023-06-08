cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(pathfinding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

find_package(ompl REQUIRED)
find_package(Boost REQUIRED COMPONENTS log_setup log)

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
  src/object.cpp
  src/obstacle.cpp
  src/car.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OMPL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OMPL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

add_library(sub::pathfinding ALIAS ${PROJECT_NAME})

add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")