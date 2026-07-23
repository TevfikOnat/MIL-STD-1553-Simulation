cmake_minimum_required(VERSION 3.20)

project(BUS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BusController
    "BusController/main.cpp"
)

add_executable(RemoteTerminal
    "RemoteTerminal/main.cpp"
)