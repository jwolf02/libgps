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
						src/gps_data.hpp)

add_library(gps STATIC ${SOURCES})
target_include_directories(gps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(gps PUBLIC m pthread)

add_executable(position_logger examples/position_logger.cpp)
target_include_directories(position_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(position_logger PUBLIC gps)

add_executable(compute_uncertainity examples/compute_uncertainity.cpp)
target_include_directories(compute_uncertainity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(compute_uncertainity PUBLIC gps)

enable_testing()

add_executable(nmea_test test/nmea_test.cpp test/test.hpp)
target_include_directories(nmea_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/test)
target_link_libraries(nmea_test gps)

add_executable(gps_test test/gps_test.cpp test/test.hpp)
target_include_directories(gps_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/test)
target_link_libraries(gps_test gps)

add_test(NAME nmea_test COMMAND nmea_test)
add_test(NAME gps_test COMMAND gps_test)