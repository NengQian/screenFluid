# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chui/Desktop/rendering

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chui/Desktop/rendering/build

# Include any dependencies generated for this target.
include glad-4.4-core/CMakeFiles/glad.dir/depend.make

# Include the progress variables for this target.
include glad-4.4-core/CMakeFiles/glad.dir/progress.make

# Include the compile flags for this target's objects.
include glad-4.4-core/CMakeFiles/glad.dir/flags.make

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o: glad-4.4-core/CMakeFiles/glad.dir/flags.make
glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o: ../glad-4.4-core/src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o"
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/glad.dir/src/glad.c.o   -c /home/chui/Desktop/rendering/glad-4.4-core/src/glad.c

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glad.dir/src/glad.c.i"
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/chui/Desktop/rendering/glad-4.4-core/src/glad.c > CMakeFiles/glad.dir/src/glad.c.i

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glad.dir/src/glad.c.s"
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/chui/Desktop/rendering/glad-4.4-core/src/glad.c -o CMakeFiles/glad.dir/src/glad.c.s

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.requires:

.PHONY : glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.requires

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.provides: glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.requires
	$(MAKE) -f glad-4.4-core/CMakeFiles/glad.dir/build.make glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.provides.build
.PHONY : glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.provides

glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.provides.build: glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o


# Object files for target glad
glad_OBJECTS = \
"CMakeFiles/glad.dir/src/glad.c.o"

# External object files for target glad
glad_EXTERNAL_OBJECTS =

glad-4.4-core/bin/libglad.a: glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o
glad-4.4-core/bin/libglad.a: glad-4.4-core/CMakeFiles/glad.dir/build.make
glad-4.4-core/bin/libglad.a: glad-4.4-core/CMakeFiles/glad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library bin/libglad.a"
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean_target.cmake
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glad.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glad-4.4-core/CMakeFiles/glad.dir/build: glad-4.4-core/bin/libglad.a

.PHONY : glad-4.4-core/CMakeFiles/glad.dir/build

glad-4.4-core/CMakeFiles/glad.dir/requires: glad-4.4-core/CMakeFiles/glad.dir/src/glad.c.o.requires

.PHONY : glad-4.4-core/CMakeFiles/glad.dir/requires

glad-4.4-core/CMakeFiles/glad.dir/clean:
	cd /home/chui/Desktop/rendering/build/glad-4.4-core && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean.cmake
.PHONY : glad-4.4-core/CMakeFiles/glad.dir/clean

glad-4.4-core/CMakeFiles/glad.dir/depend:
	cd /home/chui/Desktop/rendering/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chui/Desktop/rendering /home/chui/Desktop/rendering/glad-4.4-core /home/chui/Desktop/rendering/build /home/chui/Desktop/rendering/build/glad-4.4-core /home/chui/Desktop/rendering/build/glad-4.4-core/CMakeFiles/glad.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glad-4.4-core/CMakeFiles/glad.dir/depend

