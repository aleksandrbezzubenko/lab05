cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build examples" ON)
SET(COVERAGE OFF CACHE BOOL "Coverage")

project(banking)

add_library(banking STATIC
        banking/Account.cpp
        banking/Transaction.cpp
        )

target_include_directories(banking PUBLIC include)

if (BUILD_TESTS)
    enable_testing()
    file(GLOB BANKING_TEST_SOURCES tests/*.cpp)
    add_executable(checks ${BANKING_TEST_SOURCES})
    target_compile_options(checks PRIVATE --coverage)
    target_include_directories(checks PUBLIC tests)
    target_link_libraries(checks PRIVATE banking gtest_main gmock_main --coverage)
    add_test(NAME unit_tests COMMAND checks)
endif ()
