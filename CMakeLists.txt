cmake_minimum_required(VERSION 3.23)

project(avshell)

add_executable(avshell)

set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES src/*.c src/builtins/*.c)

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})



