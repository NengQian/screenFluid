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
include minimal/CMakeFiles/cg1-min.dir/depend.make

# Include the progress variables for this target.
include minimal/CMakeFiles/cg1-min.dir/progress.make

# Include the compile flags for this target's objects.
include minimal/CMakeFiles/cg1-min.dir/flags.make

minimal/CMakeFiles/cg1-min.dir/main.cc.o: minimal/CMakeFiles/cg1-min.dir/flags.make
minimal/CMakeFiles/cg1-min.dir/main.cc.o: ../minimal/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object minimal/CMakeFiles/cg1-min.dir/main.cc.o"
	cd /home/chui/Desktop/rendering/build/minimal && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cg1-min.dir/main.cc.o -c /home/chui/Desktop/rendering/minimal/main.cc

minimal/CMakeFiles/cg1-min.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cg1-min.dir/main.cc.i"
	cd /home/chui/Desktop/rendering/build/minimal && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chui/Desktop/rendering/minimal/main.cc > CMakeFiles/cg1-min.dir/main.cc.i

minimal/CMakeFiles/cg1-min.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cg1-min.dir/main.cc.s"
	cd /home/chui/Desktop/rendering/build/minimal && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chui/Desktop/rendering/minimal/main.cc -o CMakeFiles/cg1-min.dir/main.cc.s

minimal/CMakeFiles/cg1-min.dir/main.cc.o.requires:

.PHONY : minimal/CMakeFiles/cg1-min.dir/main.cc.o.requires

minimal/CMakeFiles/cg1-min.dir/main.cc.o.provides: minimal/CMakeFiles/cg1-min.dir/main.cc.o.requires
	$(MAKE) -f minimal/CMakeFiles/cg1-min.dir/build.make minimal/CMakeFiles/cg1-min.dir/main.cc.o.provides.build
.PHONY : minimal/CMakeFiles/cg1-min.dir/main.cc.o.provides

minimal/CMakeFiles/cg1-min.dir/main.cc.o.provides.build: minimal/CMakeFiles/cg1-min.dir/main.cc.o


# Object files for target cg1-min
cg1__min_OBJECTS = \
"CMakeFiles/cg1-min.dir/main.cc.o"

# External object files for target cg1-min
cg1__min_EXTERNAL_OBJECTS =

minimal/bin/cg1-min: minimal/CMakeFiles/cg1-min.dir/main.cc.o
minimal/bin/cg1-min: minimal/CMakeFiles/cg1-min.dir/build.make
minimal/bin/cg1-min: soil2/bin/libsoil2.a
minimal/bin/cg1-min: glfw/src/bin/libglfw3.a
minimal/bin/cg1-min: glad-4.4-core/bin/libglad.a
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/librt.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libm.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libX11.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libXrandr.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libXinerama.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libXi.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
minimal/bin/cg1-min: /usr/lib/x86_64-linux-gnu/libXcursor.so
minimal/bin/cg1-min: minimal/CMakeFiles/cg1-min.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/cg1-min"
	cd /home/chui/Desktop/rendering/build/minimal && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cg1-min.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
minimal/CMakeFiles/cg1-min.dir/build: minimal/bin/cg1-min

.PHONY : minimal/CMakeFiles/cg1-min.dir/build

minimal/CMakeFiles/cg1-min.dir/requires: minimal/CMakeFiles/cg1-min.dir/main.cc.o.requires

.PHONY : minimal/CMakeFiles/cg1-min.dir/requires

minimal/CMakeFiles/cg1-min.dir/clean:
	cd /home/chui/Desktop/rendering/build/minimal && $(CMAKE_COMMAND) -P CMakeFiles/cg1-min.dir/cmake_clean.cmake
.PHONY : minimal/CMakeFiles/cg1-min.dir/clean

minimal/CMakeFiles/cg1-min.dir/depend:
	cd /home/chui/Desktop/rendering/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chui/Desktop/rendering /home/chui/Desktop/rendering/minimal /home/chui/Desktop/rendering/build /home/chui/Desktop/rendering/build/minimal /home/chui/Desktop/rendering/build/minimal/CMakeFiles/cg1-min.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : minimal/CMakeFiles/cg1-min.dir/depend

