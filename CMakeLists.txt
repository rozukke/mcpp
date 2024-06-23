cmake_minimum_required(VERSION 3.16)

project(mcpp)

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
        PUBLIC_HEADER "${MCPP_INCLUDE_FILES}"
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)



