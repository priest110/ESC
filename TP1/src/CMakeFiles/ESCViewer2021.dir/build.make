# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src

# Include any dependencies generated for this target.
include CMakeFiles/ESCViewer2021.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ESCViewer2021.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ESCViewer2021.dir/flags.make

CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o: CMakeFiles/ESCViewer2021.dir/flags.make
CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o: scene/sceneloader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o -c /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/scene/sceneloader.cpp

CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/scene/sceneloader.cpp > CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.i

CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/scene/sceneloader.cpp -o CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.s

CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o: CMakeFiles/ESCViewer2021.dir/flags.make
CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o: main_ispc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o -c /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/main_ispc.cpp

CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/main_ispc.cpp > CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.i

CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/main_ispc.cpp -o CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.s

# Object files for target ESCViewer2021
ESCViewer2021_OBJECTS = \
"CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o" \
"CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o"

# External object files for target ESCViewer2021
ESCViewer2021_EXTERNAL_OBJECTS =

ESCViewer2021: CMakeFiles/ESCViewer2021.dir/scene/sceneloader.cpp.o
ESCViewer2021: CMakeFiles/ESCViewer2021.dir/main_ispc.cpp.o
ESCViewer2021: CMakeFiles/ESCViewer2021.dir/build.make
ESCViewer2021: CMakeFiles/ESCViewer2021.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ESCViewer2021"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ESCViewer2021.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying models"
	/usr/local/bin/cmake -E copy_directory /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/models

# Rule to build all files generated by this target.
CMakeFiles/ESCViewer2021.dir/build: ESCViewer2021

.PHONY : CMakeFiles/ESCViewer2021.dir/build

CMakeFiles/ESCViewer2021.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ESCViewer2021.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ESCViewer2021.dir/clean

CMakeFiles/ESCViewer2021.dir/depend:
	cd /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src /home/ruizinho/Desktop/Universidade/Mestrado/CPD/ESC/tracer/src/CMakeFiles/ESCViewer2021.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ESCViewer2021.dir/depend

