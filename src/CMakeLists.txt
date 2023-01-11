file(GLOB SOURCES *.cpp)
include_directories(../include)
add_library(${PROJECT_NAME} ${SOURCES})