# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build

# Include any dependencies generated for this target.
include CMakeFiles/version_qt_desk.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/version_qt_desk.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/version_qt_desk.dir/flags.make

CMakeFiles/version_qt_desk.dir/launcher.cc.o: CMakeFiles/version_qt_desk.dir/flags.make
CMakeFiles/version_qt_desk.dir/launcher.cc.o: /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk/launcher.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/version_qt_desk.dir/launcher.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/version_qt_desk.dir/launcher.cc.o -c /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk/launcher.cc

CMakeFiles/version_qt_desk.dir/launcher.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/version_qt_desk.dir/launcher.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk/launcher.cc > CMakeFiles/version_qt_desk.dir/launcher.cc.i

CMakeFiles/version_qt_desk.dir/launcher.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/version_qt_desk.dir/launcher.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk/launcher.cc -o CMakeFiles/version_qt_desk.dir/launcher.cc.s

CMakeFiles/version_qt_desk.dir/launcher.cc.o.requires:
.PHONY : CMakeFiles/version_qt_desk.dir/launcher.cc.o.requires

CMakeFiles/version_qt_desk.dir/launcher.cc.o.provides: CMakeFiles/version_qt_desk.dir/launcher.cc.o.requires
	$(MAKE) -f CMakeFiles/version_qt_desk.dir/build.make CMakeFiles/version_qt_desk.dir/launcher.cc.o.provides.build
.PHONY : CMakeFiles/version_qt_desk.dir/launcher.cc.o.provides

CMakeFiles/version_qt_desk.dir/launcher.cc.o.provides.build: CMakeFiles/version_qt_desk.dir/launcher.cc.o

# Object files for target version_qt_desk
version_qt_desk_OBJECTS = \
"CMakeFiles/version_qt_desk.dir/launcher.cc.o"

# External object files for target version_qt_desk
version_qt_desk_EXTERNAL_OBJECTS =

version_qt_desk: CMakeFiles/version_qt_desk.dir/launcher.cc.o
version_qt_desk: CMakeFiles/version_qt_desk.dir/build.make
version_qt_desk: CMakeFiles/version_qt_desk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable version_qt_desk"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/version_qt_desk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/version_qt_desk.dir/build: version_qt_desk
.PHONY : CMakeFiles/version_qt_desk.dir/build

CMakeFiles/version_qt_desk.dir/requires: CMakeFiles/version_qt_desk.dir/launcher.cc.o.requires
.PHONY : CMakeFiles/version_qt_desk.dir/requires

CMakeFiles/version_qt_desk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/version_qt_desk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/version_qt_desk.dir/clean

CMakeFiles/version_qt_desk.dir/depend:
	cd /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build /home/zaqwes/work/apps/coordinator-tasks/version-qt-desk-build/CMakeFiles/version_qt_desk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/version_qt_desk.dir/depend

