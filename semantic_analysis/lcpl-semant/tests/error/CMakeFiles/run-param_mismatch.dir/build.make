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

# Utility rule file for run-param_mismatch.

# Include the progress variables for this target.
include tests/error/CMakeFiles/run-param_mismatch.dir/progress.make

tests/error/CMakeFiles/run-param_mismatch: tests/error/param_mismatch
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && ./param_mismatch param_mismatch.lcpl.json

run-param_mismatch: tests/error/CMakeFiles/run-param_mismatch
run-param_mismatch: tests/error/CMakeFiles/run-param_mismatch.dir/build.make

.PHONY : run-param_mismatch

# Rule to build all files generated by this target.
tests/error/CMakeFiles/run-param_mismatch.dir/build: run-param_mismatch

.PHONY : tests/error/CMakeFiles/run-param_mismatch.dir/build

tests/error/CMakeFiles/run-param_mismatch.dir/clean:
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" && $(CMAKE_COMMAND) -P CMakeFiles/run-param_mismatch.dir/cmake_clean.cmake
.PHONY : tests/error/CMakeFiles/run-param_mismatch.dir/clean

tests/error/CMakeFiles/run-param_mismatch.dir/depend:
	cd "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error" "/Users/Winterfell/Documents/School/CPL/Teme/Tema 2 - Analiza Semantica/lcpl-semant/tests/error/CMakeFiles/run-param_mismatch.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/error/CMakeFiles/run-param_mismatch.dir/depend

