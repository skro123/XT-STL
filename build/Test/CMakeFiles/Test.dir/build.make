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
CMAKE_BINARY_DIR = /home/mdl/project/xt_stl/build

# Include any dependencies generated for this target.
include Test/CMakeFiles/Test.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/Test.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/Test.dir/flags.make

Test/CMakeFiles/Test.dir/test.cpp.o: Test/CMakeFiles/Test.dir/flags.make
Test/CMakeFiles/Test.dir/test.cpp.o: ../Test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mdl/project/xt_stl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/Test.dir/test.cpp.o"
	cd /home/mdl/project/xt_stl/build/Test && /usr/bin/x86_64-linux-gnu-g++-5  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Test.dir/test.cpp.o -c /home/mdl/project/xt_stl/Test/test.cpp

Test/CMakeFiles/Test.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test.dir/test.cpp.i"
	cd /home/mdl/project/xt_stl/build/Test && /usr/bin/x86_64-linux-gnu-g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mdl/project/xt_stl/Test/test.cpp > CMakeFiles/Test.dir/test.cpp.i

Test/CMakeFiles/Test.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test.dir/test.cpp.s"
	cd /home/mdl/project/xt_stl/build/Test && /usr/bin/x86_64-linux-gnu-g++-5 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mdl/project/xt_stl/Test/test.cpp -o CMakeFiles/Test.dir/test.cpp.s

# Object files for target Test
Test_OBJECTS = \
"CMakeFiles/Test.dir/test.cpp.o"

# External object files for target Test
Test_EXTERNAL_OBJECTS =

../bin/Test: Test/CMakeFiles/Test.dir/test.cpp.o
../bin/Test: Test/CMakeFiles/Test.dir/build.make
../bin/Test: lib/libgtest_main.a
../bin/Test: lib/libgtest.a
../bin/Test: Test/CMakeFiles/Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mdl/project/xt_stl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/Test"
	cd /home/mdl/project/xt_stl/build/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/mdl/project/xt_stl/build/Test && /usr/bin/cmake -D TEST_TARGET=Test -D TEST_EXECUTABLE=/home/mdl/project/xt_stl/bin/Test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/mdl/project/xt_stl/build/Test -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=Test_TESTS -D CTEST_FILE=/home/mdl/project/xt_stl/build/Test/Test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P /usr/share/cmake-3.14/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
Test/CMakeFiles/Test.dir/build: ../bin/Test

.PHONY : Test/CMakeFiles/Test.dir/build

Test/CMakeFiles/Test.dir/clean:
	cd /home/mdl/project/xt_stl/build/Test && $(CMAKE_COMMAND) -P CMakeFiles/Test.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/Test.dir/clean

Test/CMakeFiles/Test.dir/depend:
	cd /home/mdl/project/xt_stl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mdl/project/xt_stl /home/mdl/project/xt_stl/Test /home/mdl/project/xt_stl/build /home/mdl/project/xt_stl/build/Test /home/mdl/project/xt_stl/build/Test/CMakeFiles/Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/Test.dir/depend
