# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nathan/Desktop/zombie_friends

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nathan/Desktop/zombie_friends/build

# Include any dependencies generated for this target.
include src/launcher/CMakeFiles/launcher.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/launcher/CMakeFiles/launcher.dir/compiler_depend.make

# Include the progress variables for this target.
include src/launcher/CMakeFiles/launcher.dir/progress.make

# Include the compile flags for this target's objects.
include src/launcher/CMakeFiles/launcher.dir/flags.make

src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o: src/launcher/CMakeFiles/launcher.dir/flags.make
src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o: /home/nathan/Desktop/zombie_friends/src/launcher/launcher.cpp
src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o: src/launcher/CMakeFiles/launcher.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nathan/Desktop/zombie_friends/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o"
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o -MF CMakeFiles/launcher.dir/launcher.cpp.o.d -o CMakeFiles/launcher.dir/launcher.cpp.o -c /home/nathan/Desktop/zombie_friends/src/launcher/launcher.cpp

src/launcher/CMakeFiles/launcher.dir/launcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/launcher.dir/launcher.cpp.i"
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nathan/Desktop/zombie_friends/src/launcher/launcher.cpp > CMakeFiles/launcher.dir/launcher.cpp.i

src/launcher/CMakeFiles/launcher.dir/launcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/launcher.dir/launcher.cpp.s"
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nathan/Desktop/zombie_friends/src/launcher/launcher.cpp -o CMakeFiles/launcher.dir/launcher.cpp.s

# Object files for target launcher
launcher_OBJECTS = \
"CMakeFiles/launcher.dir/launcher.cpp.o"

# External object files for target launcher
launcher_EXTERNAL_OBJECTS =

lib/liblauncher.a: src/launcher/CMakeFiles/launcher.dir/launcher.cpp.o
lib/liblauncher.a: src/launcher/CMakeFiles/launcher.dir/build.make
lib/liblauncher.a: src/launcher/CMakeFiles/launcher.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nathan/Desktop/zombie_friends/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../lib/liblauncher.a"
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && $(CMAKE_COMMAND) -P CMakeFiles/launcher.dir/cmake_clean_target.cmake
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/launcher.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/launcher/CMakeFiles/launcher.dir/build: lib/liblauncher.a
.PHONY : src/launcher/CMakeFiles/launcher.dir/build

src/launcher/CMakeFiles/launcher.dir/clean:
	cd /home/nathan/Desktop/zombie_friends/build/src/launcher && $(CMAKE_COMMAND) -P CMakeFiles/launcher.dir/cmake_clean.cmake
.PHONY : src/launcher/CMakeFiles/launcher.dir/clean

src/launcher/CMakeFiles/launcher.dir/depend:
	cd /home/nathan/Desktop/zombie_friends/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nathan/Desktop/zombie_friends /home/nathan/Desktop/zombie_friends/src/launcher /home/nathan/Desktop/zombie_friends/build /home/nathan/Desktop/zombie_friends/build/src/launcher /home/nathan/Desktop/zombie_friends/build/src/launcher/CMakeFiles/launcher.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/launcher/CMakeFiles/launcher.dir/depend
