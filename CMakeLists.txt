cmake_minimum_required(VERSION 3.16)
project(MegeSandbox)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
include_directories(${RAYLIB_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(mege-sandbox ${SOURCES})
target_link_libraries(mege-sandbox raylib m)
