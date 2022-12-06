cmake_minimum_required(VERSION 3.23)
project(mcpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_library(mcpp src/library.cpp)
