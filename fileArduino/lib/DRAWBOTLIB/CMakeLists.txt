cmake_minimum_required( VERSION 3.19)
project( DRAWBOTLIB
         LANGUAGES C CXX ASM
         DESCRIPTION "Libreria per cnc" )

add_library( DRAWBOTLIB STATIC src/DRAWBOTLIB.cpp)



target_link_libraries(DRAWBOTLIB PUBLIC ArduinoCore)
target_include_directories( DRAWBOTLIB PUBLIC include )
target_include_directories( DRAWBOTLIB PUBLIC ../Servo/include )

add_library( LORLO::DRAWBOTLIB ALIAS DRAWBOTLIB ) 

#add_subdirectory(examples)
