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
include particles-2/CMakeFiles/cg1-particles2.dir/depend.make

# Include the progress variables for this target.
include particles-2/CMakeFiles/cg1-particles2.dir/progress.make

# Include the compile flags for this target's objects.
include particles-2/CMakeFiles/cg1-particles2.dir/flags.make

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o: particles-2/CMakeFiles/cg1-particles2.dir/flags.make
particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o: ../particles-2/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o"
	cd /home/chui/Desktop/rendering/build/particles-2 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cg1-particles2.dir/main.cc.o -c /home/chui/Desktop/rendering/particles-2/main.cc

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cg1-particles2.dir/main.cc.i"
	cd /home/chui/Desktop/rendering/build/particles-2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chui/Desktop/rendering/particles-2/main.cc > CMakeFiles/cg1-particles2.dir/main.cc.i

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cg1-particles2.dir/main.cc.s"
	cd /home/chui/Desktop/rendering/build/particles-2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chui/Desktop/rendering/particles-2/main.cc -o CMakeFiles/cg1-particles2.dir/main.cc.s

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.requires:

.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.requires

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.provides: particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.requires
	$(MAKE) -f particles-2/CMakeFiles/cg1-particles2.dir/build.make particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.provides.build
.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.provides

particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.provides.build: particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o


# Object files for target cg1-particles2
cg1__particles2_OBJECTS = \
"CMakeFiles/cg1-particles2.dir/main.cc.o"

# External object files for target cg1-particles2
cg1__particles2_EXTERNAL_OBJECTS =

particles-2/bin/cg1-particles2: particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o
particles-2/bin/cg1-particles2: particles-2/CMakeFiles/cg1-particles2.dir/build.make
particles-2/bin/cg1-particles2: soil2/bin/libsoil2.a
particles-2/bin/cg1-particles2: glfw/src/bin/libglfw3.a
particles-2/bin/cg1-particles2: glad-4.4-core/bin/libglad.a
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/librt.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libm.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libX11.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libXrandr.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libXinerama.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libXi.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
particles-2/bin/cg1-particles2: /usr/lib/x86_64-linux-gnu/libXcursor.so
particles-2/bin/cg1-particles2: particles-2/CMakeFiles/cg1-particles2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chui/Desktop/rendering/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/cg1-particles2"
	cd /home/chui/Desktop/rendering/build/particles-2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cg1-particles2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
particles-2/CMakeFiles/cg1-particles2.dir/build: particles-2/bin/cg1-particles2

.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/build

particles-2/CMakeFiles/cg1-particles2.dir/requires: particles-2/CMakeFiles/cg1-particles2.dir/main.cc.o.requires

.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/requires

particles-2/CMakeFiles/cg1-particles2.dir/clean:
	cd /home/chui/Desktop/rendering/build/particles-2 && $(CMAKE_COMMAND) -P CMakeFiles/cg1-particles2.dir/cmake_clean.cmake
.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/clean

particles-2/CMakeFiles/cg1-particles2.dir/depend:
	cd /home/chui/Desktop/rendering/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chui/Desktop/rendering /home/chui/Desktop/rendering/particles-2 /home/chui/Desktop/rendering/build /home/chui/Desktop/rendering/build/particles-2 /home/chui/Desktop/rendering/build/particles-2/CMakeFiles/cg1-particles2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : particles-2/CMakeFiles/cg1-particles2.dir/depend

