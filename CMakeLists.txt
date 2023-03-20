cmake_minimum_required(VERSION 3.14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(BPR_NAMI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ProcessorCount)
ProcessorCount(N)
message("Number of processors: "  ${N})
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
  set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${N}")      
endif()
message("Cmake make program" ${CMAKE_MAKE_PROGRAM})

include_directories(
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/pathfinding/include/
)

find_package(Eigen3 REQUIRED)

add_subdirectory(pathfinding)
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/myexception.cpp
    src/net.cpp
    src/data_unpacker.cpp
    src/pathfinder.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE sub::pathfinding Eigen3::Eigen)

configure_file(data/config.json ${CMAKE_BINARY_DIR}/config.json @ONLY)
configure_file(data/config_hybrid_a_star.json ${CMAKE_BINARY_DIR}/config_hybrid_a_star.json @ONLY)

add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")