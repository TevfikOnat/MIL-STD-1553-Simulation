cmake_minimum_required(VERSION 3.20)

project(FlightComputerTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(FlightComputerTests
    CommandWordTest.cpp
)

target_include_directories(FlightComputerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FlightComputerTests PRIVATE
    Common
    GTest::gtest_main
)

target_include_directories(FlightComputerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
)

target_sources(FlightComputerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common/CommandWord.cpp
)

target_link_libraries(FlightComputerTests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(FlightComputerTests)