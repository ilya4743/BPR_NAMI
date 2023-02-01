cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(net)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME}    
  src/net.cpp
  src/DataUnpacker.cpp
)

target_link_libraries(${PROJECT_NAME} ${OMPL_LIBRARIES})

add_library(sub::net ALIAS ${PROJECT_NAME})