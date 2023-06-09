cmake_minimum_required(VERSION 3.16)

project(
    create-cpp-project
    VERSION 1.0
    DESCRIPTION "Utility program for quickly creating git repo for a C++ project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(external/termcolor)
include_directories(external/CLI11/include)
include_directories(external/pstreams/pstreams-1.0.3)

add_executable(create-cpp-project ./src/main.cpp)

install(TARGETS create-cpp-project)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
