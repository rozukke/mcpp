cmake_minimum_required(VERSION 3.16)
project(mcpp)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCE_DIR src)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

#set(BOOST_ROOT "$ENV{HOME}/boost/boost")



