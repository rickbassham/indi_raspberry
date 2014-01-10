cmake_minimum_required(VERSION 2.4.7)
PROJECT(indi_raspicam C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

SET(CMAKE_CXX_FLAGS "-Wall" )
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG_FITS" )
SET(CMAKE_C_FLAGS "-Wall" )
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -DDEBUG_FITS" )

find_package(USB REQUIRED)
find_package(CFITSIO REQUIRED)
find_package(INDI REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_package(JPEG REQUIRED)

find_program(DCRAW_EXECUTABLE NAMES dcraw
    PATHS
    /usr/bin
    /usr/local/bin)

if (NOT DCRAW_EXECUTABLE)
    message(FATAL_ERROR "dcraw not found. Please install dcraw and try again.")
endif (NOT DCRAW_EXECUTABLE)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${INDI_INCLUDE_DIR})
include_directories( ${CFITSIO_INCLUDE_DIR})

########### raspicam ###########
set(indiraspicam_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/raspicam_ccd.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/raspicam_driver.c
   ${CMAKE_CURRENT_SOURCE_DIR}/raspicam_readimage.c
   )

add_executable(indi_raspicam_ccd ${indiraspicam_SRCS})

target_link_libraries(indi_raspicam_ccd ${INDI_DRIVER_LIBRARIES} ${CFITSIO_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${JPEG_LIBRARIES} ${ZLIB_LIBRARIES})

install(TARGETS indi_raspicam_ccd RUNTIME DESTINATION bin )

install(FILES indi_raspicam.xml DESTINATION ${INDI_DATA_DIR})
