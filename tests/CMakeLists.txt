# set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm@12/bin/clang++")
# set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/8a6feabf04bec8fb125e0df0ad1195c42350725f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# message(STATUS ${GTEST_LIBRARIES})

set(PROJECT_TEST_NAME lispTests)
add_executable(${PROJECT_TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Types.cpp)
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE gtest_main lisp)
enable_testing()
add_test(
    NAME ${PROJECT_TEST_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
    COMMAND ${PROJECT_TEST_NAME}
    )
