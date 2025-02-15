cmake_minimum_required(VERSION 3.16)

# Check if PROJECT_VERSION is already defined, otherwise set a default
if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION 1.0.0)
endif()

project(mcpp VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

# Used for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(MCPP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MCPP_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Testing
add_subdirectory(test)
enable_testing()
add_test(NAME local COMMAND local_tests)
add_test(NAME full COMMAND test_suite)

# Source files
file(GLOB_RECURSE MCPP_INCLUDE_FILES ${MCPP_INC_DIR}/*.h)
file(GLOB_RECURSE MCPP_SOURCE_FILES ${MCPP_SRC_DIR}/*.cpp)

# Library build
add_library(${PROJECT_NAME} SHARED ${MCPP_INCLUDE_FILES} ${MCPP_SOURCE_FILES})

set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${MCPP_INCLUDE_FILES}"
    INSTALL_NAME_DIR ${LIB_INSTALL_DIR}
)

# Fix silly macOS include errors
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${MCPP_INC_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_DIR}
)

# Build examples
add_executable(hello_minecraft EXCLUDE_FROM_ALL example/hello_minecraft.cpp)
add_executable(pyramid EXCLUDE_FROM_ALL example/pyramid.cpp)
add_executable(game_of_life EXCLUDE_FROM_ALL example/game-of-life/game_of_life.cpp)
add_executable(minesweeper EXCLUDE_FROM_ALL example/minesweeper/minesweeper.cpp)
add_executable(video_mc EXCLUDE_FROM_ALL example/video-generation/video-mc.cpp)
add_executable(obj_mc EXCLUDE_FROM_ALL example/model-generation/obj-mc.cpp)

target_link_libraries(hello_minecraft ${PROJECT_NAME})
target_link_libraries(pyramid ${PROJECT_NAME})
target_link_libraries(game_of_life ${PROJECT_NAME})
target_link_libraries(minesweeper ${PROJECT_NAME})
target_link_libraries(video_mc ${PROJECT_NAME})
target_link_libraries(obj_mc ${PROJECT_NAME})

add_custom_target(examples DEPENDS hello_minecraft pyramid game_of_life minesweeper video_mc obj_mc)


