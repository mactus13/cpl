# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant"

# Include any dependencies generated for this target.
include tests/error/CMakeFiles/met_over.dir/depend.make

# Include the progress variables for this target.
include tests/error/CMakeFiles/met_over.dir/progress.make

# Include the compile flags for this target's objects.
include tests/error/CMakeFiles/met_over.dir/flags.make

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o: tests/error/CMakeFiles/met_over.dir/flags.make
tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o: tests/TestRig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/met_over.dir/__/TestRig.cpp.o -c "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/TestRig.cpp"

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/met_over.dir/__/TestRig.cpp.i"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/TestRig.cpp" > CMakeFiles/met_over.dir/__/TestRig.cpp.i

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/met_over.dir/__/TestRig.cpp.s"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/TestRig.cpp" -o CMakeFiles/met_over.dir/__/TestRig.cpp.s

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.requires:

.PHONY : tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.requires

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.provides: tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.requires
	$(MAKE) -f tests/error/CMakeFiles/met_over.dir/build.make tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.provides.build
.PHONY : tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.provides

tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.provides.build: tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o


tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o: tests/error/CMakeFiles/met_over.dir/flags.make
tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o: tests/error/met_overTestRig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/met_over.dir/met_overTestRig.cpp.o -c "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error/met_overTestRig.cpp"

tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/met_over.dir/met_overTestRig.cpp.i"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5  $(CXX_DEFINES) $(CXX_FLAGS) -E "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error/met_overTestRig.cpp" > CMakeFiles/met_over.dir/met_overTestRig.cpp.i

tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/met_over.dir/met_overTestRig.cpp.s"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && /usr/local/bin/g++-5  $(CXX_DEFINES) $(CXX_FLAGS) -S "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error/met_overTestRig.cpp" -o CMakeFiles/met_over.dir/met_overTestRig.cpp.s

tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.requires:

.PHONY : tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.requires

tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.provides: tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.requires
	$(MAKE) -f tests/error/CMakeFiles/met_over.dir/build.make tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.provides.build
.PHONY : tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.provides

tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.provides.build: tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o


# Object files for target met_over
met_over_OBJECTS = \
"CMakeFiles/met_over.dir/__/TestRig.cpp.o" \
"CMakeFiles/met_over.dir/met_overTestRig.cpp.o"

# External object files for target met_over
met_over_EXTERNAL_OBJECTS =

tests/error/met_over: tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o
tests/error/met_over: tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o
tests/error/met_over: tests/error/CMakeFiles/met_over.dir/build.make
tests/error/met_over: liblcpl-semant.a
tests/error/met_over: tests/error/CMakeFiles/met_over.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable met_over"
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/met_over.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/error/CMakeFiles/met_over.dir/build: tests/error/met_over

.PHONY : tests/error/CMakeFiles/met_over.dir/build

tests/error/CMakeFiles/met_over.dir/requires: tests/error/CMakeFiles/met_over.dir/__/TestRig.cpp.o.requires
tests/error/CMakeFiles/met_over.dir/requires: tests/error/CMakeFiles/met_over.dir/met_overTestRig.cpp.o.requires

.PHONY : tests/error/CMakeFiles/met_over.dir/requires

tests/error/CMakeFiles/met_over.dir/clean:
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && $(CMAKE_COMMAND) -P CMakeFiles/met_over.dir/cmake_clean.cmake
.PHONY : tests/error/CMakeFiles/met_over.dir/clean

tests/error/CMakeFiles/met_over.dir/depend:
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error/CMakeFiles/met_over.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/error/CMakeFiles/met_over.dir/depend

