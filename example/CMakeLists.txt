cmake_minimum_required(VERSION 3.15)
project(example)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_CXX_STANDARD 14)
# The line below is necessary if you are under Windows only
# Comment the line below if you are under Linux or Mac OS
# link_libraries(ws2_32 wsock32)

add_subdirectory(.. build)

include_directories(../include)

add_executable(example main.cpp)

target_link_libraries(${PROJECT_NAME} graphviewer)
