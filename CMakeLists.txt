cmake_minimum_required(VERSION 3.15)

project(lisp VERSION 1.0)


set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm@12/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fsanitize=leak -fno-omit-frame-pointer ")

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Env.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Env.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Eval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Eval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Procedure.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Types.h
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
