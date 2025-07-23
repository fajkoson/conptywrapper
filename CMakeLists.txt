cmake_minimum_required(VERSION 3.20)
project(
    conptywrapper 
    LANGUAGES CXX
)
include(cmake/macros.cmake)
version()
author()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

add_subdirectory(source/example)
add_subdirectory(source/pseudoconsole)
add_subdirectory(build-scripts/buildWheel)


message(STATUS "")
message(STATUS "Version......: ${PROJECT_VERSION}")
message(STATUS "Author.......: ${AUTHOR}")
message(STATUS "")