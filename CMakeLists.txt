cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(BPR_NAMI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/pathfinding/include/
    include ${OMPL_INCLUDE_DIRS}
)

add_subdirectory(pathfinding)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/myexception.cpp
    src/net.cpp
    src/data_unpacker.cpp
    src/pathfinder.cpp
)

target_link_libraries(${PROJECT_NAME} sub::pathfinding)

configure_file(data/config.json ${CMAKE_BINARY_DIR}/config.json @ONLY)
configure_file(data/config_hybrid_a_star.json ${CMAKE_BINARY_DIR}/config_hybrid_a_star.json @ONLY)