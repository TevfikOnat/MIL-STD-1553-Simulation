cmake_minimum_required(VERSION 3.20)

project(BUS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Common 
Common/CommandWord.cpp)

target_include_directories(Common PUBLIC Common)

add_executable(BusController
    "BusController/main.cpp")

add_executable(RemoteTerminal
    "RemoteTerminal/main.cpp")
 
target_include_directories(BusController PRIVATE Common)
target_include_directories(RemoteTerminal PRIVATE Common)

target_link_libraries(BusController PRIVATE Common)
target_link_libraries(RemoteTerminal PRIVATE Common)