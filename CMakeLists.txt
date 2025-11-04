cmake_minimum_required(VERSION 3.16)
project(GroverVsClassical CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-O3 -ffast-math -funroll-loops -march=native)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC_FILES src/*.cpp)
add_executable(grover_vs_classical ${SRC_FILES})
