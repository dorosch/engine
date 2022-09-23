cmake_minimum_required(VERSION 3.4)

project(app LANGUAGES CXX)

file(
    GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

# TODO: Find a better way to add a main project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../radian/include)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../radian ${CMAKE_CURRENT_BINARY_DIR}/radian)
target_link_libraries(${CMAKE_PROJECT_NAME} radian)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wall -Wextra -pedantic)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE CXX_EXTENSIONS FALSE)
