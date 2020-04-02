cmake_minimum_required(VERSION 3.4)

project(frameplayer C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)

find_package(SDL2 REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET libsdl2)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}}")

add_executable(player
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/format.cpp
  ${PROJECT_SOURCE_DIR}/codec.cpp
  ${PROJECT_SOURCE_DIR}/packet.cpp
  ${PROJECT_SOURCE_DIR}/sdlscreen.cpp
  ${PROJECT_SOURCE_DIR}/stream.cpp
  ${PROJECT_SOURCE_DIR}/swscaler.cpp
  ${PROJECT_SOURCE_DIR}/timer.cpp
)
