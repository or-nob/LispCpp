cmake_minimum_required(VERSION 3.15)

project(lisp VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm@12/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fsanitize=leak -fno-omit-frame-pointer ")

set(LIB_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Tokenizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Env.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Env.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Eval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Eval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Types.cpp
    )

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${LIB_SRC_FILES}
    )

add_executable(${PROJECT_NAME}Lib ${SOURCE_FILES})
add_library(${PROJECT_NAME} STATIC ${LIB_SRC_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

option(BUILD_TESTS OFF)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
