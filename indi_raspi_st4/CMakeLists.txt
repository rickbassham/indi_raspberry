cmake_minimum_required(VERSION 2.4.7)
PROJECT(indi_raspi_st4 C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

SET(CMAKE_CXX_FLAGS "-Wall" )
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )
SET(CMAKE_C_FLAGS "-Wall" )
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g" )

find_package(INDI REQUIRED)
find_package(WIRINGPI REQUIRED)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${INDI_INCLUDE_DIR})
include_directories( ${WIRINGPI_INCLUDE_DIR})

########### raspi_st4 ###########
set(indiraspi_st4_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/rpist4.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/rpist4driver.cpp
   )

add_executable(indi_raspi_st4 ${indiraspi_st4_SRCS})

target_link_libraries(indi_raspi_st4 ${INDI_DRIVER_LIBRARIES} ${WIRINGPI_LIBRARIES})

install(TARGETS indi_raspi_st4 RUNTIME DESTINATION bin )
