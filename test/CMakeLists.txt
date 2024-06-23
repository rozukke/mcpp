add_executable(local_tests EXCLUDE_FROM_ALL local_tests.cpp)
add_executable(minecraft_tests EXCLUDE_FROM_ALL local_tests.cpp minecraft_tests.cpp)
add_executable(test_suite EXCLUDE_FROM_ALL local_tests.cpp minecraft_tests.cpp)

target_link_libraries(local_tests ${PROJECT_NAME})
target_link_libraries(minecraft_tests ${PROJECT_NAME})
target_link_libraries(test_suite ${PROJECT_NAME})

# Enable player tests for full test suite
set_property(TARGET test_suite PROPERTY COMPILE_DEFINITIONS PLAYER_TEST)
