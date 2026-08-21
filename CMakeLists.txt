cmake_minimum_required(VERSION 3.20)

project(FlightComputer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Common 
Common/CommandWord.cpp Common/StatusWord.cpp Common/ARINCWord.cpp )

target_include_directories(Common PUBLIC Common)

add_executable(RemoteTerminal
    "MIL-STD-1553/RemoteTerminal/main.cpp" "MIL-STD-1553/RemoteTerminal/RemoteTerminal.cpp" )
 
add_executable(BUS
    "MIL-STD-1553/BUS/main.cpp" "MIL-STD-1553/BUS/BUS.cpp" )
     
add_executable(BusMonitor
    "MIL-STD-1553/BusMonitor/main.cpp" "MIL-STD-1553/BusMonitor/BusMonitor.cpp" )

add_executable(Display
    "ARINC429/Display/main.cpp" "ARINC429/Display/Display.cpp" )

add_executable(GPS
    "ARINC429/GPS/main.cpp"  "ARINC429/GPS/GPS.cpp" )

add_executable(FlightComputer
    "FlightComputer/main.cpp"  "FlightComputer/FlightComputer.cpp")

add_executable(ADC 
    "ARINC429/ADC/main.cpp"  "ARINC429/ADC/ADC.cpp")  
    
add_executable(IRS 
    "ARINC429/IRS/main.cpp"  "ARINC429/IRS/IRS.cpp")

add_executable(Radio
    "ARINC429/Radio/main.cpp" "ARINC429/Radio/Radio.cpp")

add_executable(Launcher 
    "Launcher/main.cpp"  "Launcher/Launcher.cpp")


target_include_directories(RemoteTerminal PRIVATE Common)
target_include_directories(BUS PRIVATE Common)
target_include_directories(BusMonitor PRIVATE Common)
target_include_directories(GPS PRIVATE Common)
target_include_directories(Display PRIVATE Common)
target_include_directories(FlightComputer PRIVATE Common)
target_include_directories(ADC PRIVATE Common)
target_include_directories(IRS PRIVATE Common)
target_include_directories(Radio PRIVATE Common)
target_include_directories(Launcher PRIVATE Common)



target_link_libraries(RemoteTerminal PRIVATE Common)
target_link_libraries(BUS PRIVATE Common)
target_link_libraries(BusMonitor PRIVATE Common)
target_link_libraries(GPS PRIVATE Common)
target_link_libraries(Display PRIVATE Common)
target_link_libraries(FlightComputer PRIVATE Common)
target_link_libraries(ADC PRIVATE Common)
target_link_libraries(IRS PRIVATE Common)
target_link_libraries(Radio PRIVATE Common)
target_link_libraries(Launcher PRIVATE Common)

# -------------------------------
# GoogleTest
# -------------------------------

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(FlightComputerTests
    "Tests/MIL-STD-1553_Test.cpp"
    "Tests/ARINC429_Test.cpp"
)

target_include_directories(FlightComputerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FlightComputerTests PRIVATE
    Common
    GTest::gtest_main
)


target_link_libraries(FlightComputerTests
    PRIVATE
    Common
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(FlightComputerTests)


