cmake_minimum_required(VERSION 3.14)

project(BPR_NAMI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(OGRE REQUIRED CONFIG)

add_executable(BPR_NAMI
  main.cpp
  barrier.cpp
  car.cpp
  gamemap.cpp
  locationmap.cpp
  myexception.cpp
  mygraph.cpp
  mytcpsocket.cpp
  settings.cpp
  logger.cpp
)

target_link_libraries(BPR_NAMI Qt5::Core Qt5::Network OgreMain OgreBites)
