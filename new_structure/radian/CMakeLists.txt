cmake_minimum_required(VERSION 3.4)

project(radian VERSION 1.0 LANGUAGES CXX)

file(
    GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
file(
    GLOB_RECURSE PUBLIC_HEADERS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

configure_file(
    "${PROJECT_SOURCE_DIR}/include/radian.hpp.in"
    "${PROJECT_SOURCE_DIR}/include/radian.hpp"
)

add_library(radian SHARED ${SOURCES})
target_compile_options(radian PUBLIC -Wall -Wextra -pedantic)
target_include_directories(radian PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(radian PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE CXX_EXTENSIONS FALSE)
set_target_properties(radian PROPERTIES PUBLIC_HEADER ${PUBLIC_HEADERS})

install(
    TARGETS radian 
    LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    PUBLIC_HEADER DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/include"
)
