# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\liuyuelin\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\181.5087.36\bin\cmake\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\liuyuelin\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\181.5087.36\bin\cmake\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug

# Include any dependencies generated for this target.
include examples/CMakeFiles/example1.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/example1.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/example1.dir/flags.make

examples/CMakeFiles/example1.dir/example1.cpp.obj: examples/CMakeFiles/example1.dir/flags.make
examples/CMakeFiles/example1.dir/example1.cpp.obj: examples/CMakeFiles/example1.dir/includes_CXX.rsp
examples/CMakeFiles/example1.dir/example1.cpp.obj: ../examples/example1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/example1.dir/example1.cpp.obj"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\example1.dir\example1.cpp.obj -c D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\examples\example1.cpp

examples/CMakeFiles/example1.dir/example1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example1.dir/example1.cpp.i"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\examples\example1.cpp > CMakeFiles\example1.dir\example1.cpp.i

examples/CMakeFiles/example1.dir/example1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example1.dir/example1.cpp.s"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\examples\example1.cpp -o CMakeFiles\example1.dir\example1.cpp.s

examples/CMakeFiles/example1.dir/example1.cpp.obj.requires:

.PHONY : examples/CMakeFiles/example1.dir/example1.cpp.obj.requires

examples/CMakeFiles/example1.dir/example1.cpp.obj.provides: examples/CMakeFiles/example1.dir/example1.cpp.obj.requires
	$(MAKE) -f examples\CMakeFiles\example1.dir\build.make examples/CMakeFiles/example1.dir/example1.cpp.obj.provides.build
.PHONY : examples/CMakeFiles/example1.dir/example1.cpp.obj.provides

examples/CMakeFiles/example1.dir/example1.cpp.obj.provides.build: examples/CMakeFiles/example1.dir/example1.cpp.obj


# Object files for target example1
example1_OBJECTS = \
"CMakeFiles/example1.dir/example1.cpp.obj"

# External object files for target example1
example1_EXTERNAL_OBJECTS =

examples/example1.exe: examples/CMakeFiles/example1.dir/example1.cpp.obj
examples/example1.exe: examples/CMakeFiles/example1.dir/build.make
examples/example1.exe: examples/CMakeFiles/example1.dir/linklibs.rsp
examples/example1.exe: examples/CMakeFiles/example1.dir/objects1.rsp
examples/example1.exe: examples/CMakeFiles/example1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example1.exe"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\example1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/example1.dir/build: examples/example1.exe

.PHONY : examples/CMakeFiles/example1.dir/build

examples/CMakeFiles/example1.dir/requires: examples/CMakeFiles/example1.dir/example1.cpp.obj.requires

.PHONY : examples/CMakeFiles/example1.dir/requires

examples/CMakeFiles/example1.dir/clean:
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples && $(CMAKE_COMMAND) -P CMakeFiles\example1.dir\cmake_clean.cmake
.PHONY : examples/CMakeFiles/example1.dir/clean

examples/CMakeFiles/example1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\examples D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\examples\CMakeFiles\example1.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/example1.dir/depend
