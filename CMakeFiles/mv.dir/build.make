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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wsy/Documents/sh

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wsy/Documents/sh

# Include any dependencies generated for this target.
include CMakeFiles/mv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mv.dir/flags.make

CMakeFiles/mv.dir/filecmd/mv.c.o: CMakeFiles/mv.dir/flags.make
CMakeFiles/mv.dir/filecmd/mv.c.o: filecmd/mv.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wsy/Documents/sh/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mv.dir/filecmd/mv.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mv.dir/filecmd/mv.c.o -c /home/wsy/Documents/sh/filecmd/mv.c

CMakeFiles/mv.dir/filecmd/mv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mv.dir/filecmd/mv.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wsy/Documents/sh/filecmd/mv.c > CMakeFiles/mv.dir/filecmd/mv.c.i

CMakeFiles/mv.dir/filecmd/mv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mv.dir/filecmd/mv.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wsy/Documents/sh/filecmd/mv.c -o CMakeFiles/mv.dir/filecmd/mv.c.s

# Object files for target mv
mv_OBJECTS = \
"CMakeFiles/mv.dir/filecmd/mv.c.o"

# External object files for target mv
mv_EXTERNAL_OBJECTS =

bin/mv: CMakeFiles/mv.dir/filecmd/mv.c.o
bin/mv: CMakeFiles/mv.dir/build.make
bin/mv: CMakeFiles/mv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wsy/Documents/sh/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/mv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mv.dir/build: bin/mv

.PHONY : CMakeFiles/mv.dir/build

CMakeFiles/mv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mv.dir/clean

CMakeFiles/mv.dir/depend:
	cd /home/wsy/Documents/sh && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wsy/Documents/sh /home/wsy/Documents/sh /home/wsy/Documents/sh /home/wsy/Documents/sh /home/wsy/Documents/sh/CMakeFiles/mv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mv.dir/depend
