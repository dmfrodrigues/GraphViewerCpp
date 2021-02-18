cmake_minimum_required(VERSION 3.15)
project(example)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    message("Detected WIN32")
    include_directories("$ENV{SFML_PATH}/include")
    link_directories("$ENV{SFML_PATH}/lib")
endif()

add_subdirectory(.. build)

include_directories(../include)

add_executable(example main.cpp)

target_link_libraries(${PROJECT_NAME} graphviewer)
