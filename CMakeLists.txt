cmake_minimum_required(VERSION 3.5.1)

project(libgps LANGUAGES CXX)

set(CMAKE_CXX_STANDARD	11)

set(CMAKE_CXX_FLAGS		"${CMAKE_CXX_FLAGS} -O3 -Wall -mtune=native -march=native")

set(SOURCES 			src/GPS.hpp
						src/GPS.cpp
						src/serial.hpp
						src/serial.cpp
						src/nmea.hpp
						src/nmea.cpp
						src/gps_data.hpp src/error.hpp)

add_library(gps STATIC ${SOURCES})
target_include_directories(gps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(gps PUBLIC m pthread)

add_executable(logger src/main.cpp)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(logger PUBLIC gps)