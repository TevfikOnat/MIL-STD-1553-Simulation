cmake_minimum_required(VERSION 3.20)

project(BUS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Common 
Common/CommandWord.cpp Common/StatusWord.cpp Common/ARINCWord.cpp )

target_include_directories(Common PUBLIC Common)

add_executable(BusController
    "MIL-STD-1553/BusController/main.cpp" "MIL-STD-1553/BusController/BusController.cpp" )

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

add_executable(MissionComputer
    "MissionComputer/main.cpp"  )

add_executable(ADC 
    "ARINC429/ADC/main.cpp"  "ARINC429/ADC/ADC.cpp")  
    
add_executable(IRS 
    "ARINC429/IRS/main.cpp"  "ARINC429/IRS/IRS.cpp")

add_executable(Radio
    "ARINC429/Radio/main.cpp" "ARINC429/Radio/Radio.cpp")


target_include_directories(BusController PRIVATE Common)
target_include_directories(RemoteTerminal PRIVATE Common)
target_include_directories(BUS PRIVATE Common)
target_include_directories(BusMonitor PRIVATE Common)
target_include_directories(GPS PRIVATE Common)
target_include_directories(Display PRIVATE Common)
target_include_directories(MissionComputer PRIVATE Common)
target_include_directories(ADC PRIVATE Common)
target_include_directories(IRS PRIVATE Common)
target_include_directories(Radio PRIVATE Common)



target_link_libraries(BusController PRIVATE Common)
target_link_libraries(RemoteTerminal PRIVATE Common)
target_link_libraries(BUS PRIVATE Common)
target_link_libraries(BusMonitor PRIVATE Common)
target_link_libraries(GPS PRIVATE Common)
target_link_libraries(Display PRIVATE Common)
target_link_libraries(MissionComputer PRIVATE Common)
target_link_libraries(ADC PRIVATE Common)
target_link_libraries(IRS PRIVATE Common)
target_link_libraries(Radio PRIVATE Common)


