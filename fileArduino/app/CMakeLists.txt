cmake_minimum_required( VERSION 3.19)
project( DrawBot_control
         VERSION 1.0
         DESCRIPTION "Un'applicazione per usare la cnc." )
add_executable( DrawBot src/DrawBot_control.cpp)
target_compile_options(DrawBot PUBLIC
    "-Wall"
    "-Wextra"
    "-pedantic"
)
arduino_avr_hex(DrawBot)
target_link_libraries(DrawBot PUBLIC ArduinoCore)
target_link_libraries( DrawBot PUBLIC LII::DRAWBOTLIB )
target_link_libraries( DrawBot PRIVATE LII::Servo )
