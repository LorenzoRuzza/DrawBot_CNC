cmake_minimum_required(VERSION 3.19)
project(DrawBot_control LANGUAGES C CXX ASM)

include(cmake/lto.cmake)
include(cmake/arduino_core.cmake)
include(cmake/arduino_hex.cmake)
include(cmake/arduino_upload.cmake)


set(ARDUINO_PORT "/dev/ttyACM0"
    CACHE STRING "The serial port for uploading using avrdude")
arduino_avr_upload(DrawBot ${ARDUINO_PORT})


add_subdirectory( lib )

add_subdirectory( app )
