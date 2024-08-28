cmake_minimum_required(VERSION 3.16)

# Check if PROJECT_VERSION is already defined, otherwise set a default
if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION 1.0.0)
endif()

project(mcpp VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(MCPP_SRC_DIR src)
set(MCPP_INC_DIR include/mcpp)

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

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${MCPP_INCLUDE_FILES}"
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

# CPack setup
SET(CPACK_GENERATOR "deb")
SET(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rozukke")
INCLUDE(CPack)