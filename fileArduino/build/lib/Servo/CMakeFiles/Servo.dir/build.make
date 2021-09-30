# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/cmake/936/bin/cmake

# The command to remove a file.
RM = /snap/cmake/936/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build

# Include any dependencies generated for this target.
include lib/Servo/CMakeFiles/Servo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/Servo/CMakeFiles/Servo.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/Servo/CMakeFiles/Servo.dir/progress.make

# Include the compile flags for this target's objects.
include lib/Servo/CMakeFiles/Servo.dir/flags.make

lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o: lib/Servo/CMakeFiles/Servo.dir/flags.make
lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o: ../lib/Servo/src/Servo.cpp
lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o: lib/Servo/CMakeFiles/Servo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o"
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && /home/the_lorlito/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o -MF CMakeFiles/Servo.dir/src/Servo.cpp.o.d -o CMakeFiles/Servo.dir/src/Servo.cpp.o -c /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/lib/Servo/src/Servo.cpp

lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Servo.dir/src/Servo.cpp.i"
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && /home/the_lorlito/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/lib/Servo/src/Servo.cpp > CMakeFiles/Servo.dir/src/Servo.cpp.i

lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Servo.dir/src/Servo.cpp.s"
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && /home/the_lorlito/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/lib/Servo/src/Servo.cpp -o CMakeFiles/Servo.dir/src/Servo.cpp.s

# Object files for target Servo
Servo_OBJECTS = \
"CMakeFiles/Servo.dir/src/Servo.cpp.o"

# External object files for target Servo
Servo_EXTERNAL_OBJECTS =

lib/Servo/libServo.a: lib/Servo/CMakeFiles/Servo.dir/src/Servo.cpp.o
lib/Servo/libServo.a: lib/Servo/CMakeFiles/Servo.dir/build.make
lib/Servo/libServo.a: lib/Servo/CMakeFiles/Servo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libServo.a"
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && $(CMAKE_COMMAND) -P CMakeFiles/Servo.dir/cmake_clean_target.cmake
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Servo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/Servo/CMakeFiles/Servo.dir/build: lib/Servo/libServo.a
.PHONY : lib/Servo/CMakeFiles/Servo.dir/build

lib/Servo/CMakeFiles/Servo.dir/clean:
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo && $(CMAKE_COMMAND) -P CMakeFiles/Servo.dir/cmake_clean.cmake
.PHONY : lib/Servo/CMakeFiles/Servo.dir/clean

lib/Servo/CMakeFiles/Servo.dir/depend:
	cd /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/lib/Servo /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo /home/the_lorlito/Documenti/DrawBot_CNC/fileArduino/build/lib/Servo/CMakeFiles/Servo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/Servo/CMakeFiles/Servo.dir/depend

