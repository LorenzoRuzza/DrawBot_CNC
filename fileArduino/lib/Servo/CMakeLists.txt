cmake_minimum_required( VERSION 3.19)
project( Servo
         LANGUAGES C CXX ASM
         DESCRIPTION "Libreria per asse Z cnc" )
add_library( Servo STATIC src/Servo.cpp)


target_link_libraries(Servo PUBLIC ArduinoCore)
target_include_directories( Servo PUBLIC include )


add_library( LORLO::Servo ALIAS Servo )

