cmake_minimum_required(VERSION 4.1)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} raylib)