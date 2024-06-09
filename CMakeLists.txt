cmake_minimum_required(VERSION 3.16)

project(mcpp)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCE_DIR src)
set(MCPP_PUBLIC_INCLUDE_DIRECTORIES include/ )

# Testing
enable_testing()
add_subdirectory(test)
add_test(NAME local COMMAND local_tests)
add_test(NAME full COMMAND test_suite)

# Source files
file(GLOB_RECURSE MCPP_INCLUDE_FILES "include/*.h")
file(GLOB_RECURSE MCPP_SOURCE_FILES "src/*.cpp")

# Library build
add_library(${PROJECT_NAME} STATIC ${MCPP_INCLUDE_FILES} ${MCPP_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${MCPP_PUBLIC_INCLUDE_DIRECTORIES})

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER "${MCPP_INCLUDE_FILES}"
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)



