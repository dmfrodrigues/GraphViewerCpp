cmake_minimum_required(VERSION 3.10)

project(graphviewer)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

include_directories(../include)

add_library(graphviewer STATIC graphviewer.cpp)

target_compile_options(graphviewer PRIVATE ${CMAKE_CXX_LIB})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} X11)
