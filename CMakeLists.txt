project(LEDBar)
cmake_minimum_required(VERSION 2.8)

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Locate libraries and headers
find_package(WiringPiCpp REQUIRED) # WiringPi-Cpp

# Include headers
include_directories(${WIRINGPICPP_INCLUDE_DIRS}) # WiringPi-Cpp

# ----------
# BUILD MAIN
# ----------
message(STATUS "NOTICE: Configuring demonstrator main build.")

# All paths must start with "src/"
add_executable(main
  LED_Bar_Main.cpp
  gpio.cpp
  LED_Bar_GPIO.cpp
)

# Link against libraries
target_link_libraries(main ${WIRINGPICPP_LIBRARIES}) # WiringPi-Cpp

add_executable(lowLevelTest
  lowLevelTest.cpp
)

# Link against libraries
target_link_libraries(lowLevelTest ${WIRINGPICPP_LIBRARIES}) # WiringPi-Cpp
