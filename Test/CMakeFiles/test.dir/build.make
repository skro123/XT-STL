# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_SOURCE_DIR = /home/mdl/project/xt_stl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mdl/project/xt_stl

# Include any dependencies generated for this target.
include Test/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/test.dir/flags.make

Test/CMakeFiles/test.dir/test.cpp.o: Test/CMakeFiles/test.dir/flags.make
Test/CMakeFiles/test.dir/test.cpp.o: Test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mdl/project/xt_stl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/test.dir/test.cpp.o"
	cd /home/mdl/project/xt_stl/Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/test.cpp.o -c /home/mdl/project/xt_stl/Test/test.cpp

Test/CMakeFiles/test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/test.cpp.i"
	cd /home/mdl/project/xt_stl/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mdl/project/xt_stl/Test/test.cpp > CMakeFiles/test.dir/test.cpp.i

Test/CMakeFiles/test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/test.cpp.s"
	cd /home/mdl/project/xt_stl/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mdl/project/xt_stl/Test/test.cpp -o CMakeFiles/test.dir/test.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/test.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

bin/test: Test/CMakeFiles/test.dir/test.cpp.o
bin/test: Test/CMakeFiles/test.dir/build.make
bin/test: lib/libgtest_main.a
bin/test: lib/libgtest.a
bin/test: Test/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mdl/project/xt_stl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test"
	cd /home/mdl/project/xt_stl/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/mdl/project/xt_stl/Test && /usr/bin/cmake -D TEST_TARGET=test -D TEST_EXECUTABLE=/home/mdl/project/xt_stl/bin/test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/mdl/project/xt_stl/Test -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=test_TESTS -D CTEST_FILE=/home/mdl/project/xt_stl/Test/test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P /usr/share/cmake-3.14/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
Test/CMakeFiles/test.dir/build: bin/test

.PHONY : Test/CMakeFiles/test.dir/build

Test/CMakeFiles/test.dir/clean:
	cd /home/mdl/project/xt_stl/Test && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/test.dir/clean

Test/CMakeFiles/test.dir/depend:
	cd /home/mdl/project/xt_stl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mdl/project/xt_stl /home/mdl/project/xt_stl/Test /home/mdl/project/xt_stl /home/mdl/project/xt_stl/Test /home/mdl/project/xt_stl/Test/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/test.dir/depend

