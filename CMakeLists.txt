project(LEDBar)
cmake_minimum_required(VERSION 2.8)

# Set output directories
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Enable C++11 features
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# ----------
# BUILD MAIN
# ----------
message(STATUS "NOTICE: Configuring demonstrator main build.")

# All paths must start with "src/"
add_executable(main
  gpio.cpp
  LED_Bar_GPIO.cpp
)

# Link against libraries
target_link_libraries(main ${WIRINGPICPP_LIBRARIES}) # WiringPi-Cpp
