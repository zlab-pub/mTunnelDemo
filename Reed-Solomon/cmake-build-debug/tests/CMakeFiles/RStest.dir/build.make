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
include tests/CMakeFiles/RStest.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/RStest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/RStest.dir/flags.make

tests/CMakeFiles/RStest.dir/gftest.cpp.obj: tests/CMakeFiles/RStest.dir/flags.make
tests/CMakeFiles/RStest.dir/gftest.cpp.obj: tests/CMakeFiles/RStest.dir/includes_CXX.rsp
tests/CMakeFiles/RStest.dir/gftest.cpp.obj: ../tests/gftest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/RStest.dir/gftest.cpp.obj"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\RStest.dir\gftest.cpp.obj -c D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\gftest.cpp

tests/CMakeFiles/RStest.dir/gftest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RStest.dir/gftest.cpp.i"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\gftest.cpp > CMakeFiles\RStest.dir\gftest.cpp.i

tests/CMakeFiles/RStest.dir/gftest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RStest.dir/gftest.cpp.s"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\gftest.cpp -o CMakeFiles\RStest.dir\gftest.cpp.s

tests/CMakeFiles/RStest.dir/gftest.cpp.obj.requires:

.PHONY : tests/CMakeFiles/RStest.dir/gftest.cpp.obj.requires

tests/CMakeFiles/RStest.dir/gftest.cpp.obj.provides: tests/CMakeFiles/RStest.dir/gftest.cpp.obj.requires
	$(MAKE) -f tests\CMakeFiles\RStest.dir\build.make tests/CMakeFiles/RStest.dir/gftest.cpp.obj.provides.build
.PHONY : tests/CMakeFiles/RStest.dir/gftest.cpp.obj.provides

tests/CMakeFiles/RStest.dir/gftest.cpp.obj.provides.build: tests/CMakeFiles/RStest.dir/gftest.cpp.obj


tests/CMakeFiles/RStest.dir/performancetest.cpp.obj: tests/CMakeFiles/RStest.dir/flags.make
tests/CMakeFiles/RStest.dir/performancetest.cpp.obj: tests/CMakeFiles/RStest.dir/includes_CXX.rsp
tests/CMakeFiles/RStest.dir/performancetest.cpp.obj: ../tests/performancetest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/RStest.dir/performancetest.cpp.obj"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\RStest.dir\performancetest.cpp.obj -c D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\performancetest.cpp

tests/CMakeFiles/RStest.dir/performancetest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RStest.dir/performancetest.cpp.i"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\performancetest.cpp > CMakeFiles\RStest.dir\performancetest.cpp.i

tests/CMakeFiles/RStest.dir/performancetest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RStest.dir/performancetest.cpp.s"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\performancetest.cpp -o CMakeFiles\RStest.dir\performancetest.cpp.s

tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.requires:

.PHONY : tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.requires

tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.provides: tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.requires
	$(MAKE) -f tests\CMakeFiles\RStest.dir\build.make tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.provides.build
.PHONY : tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.provides

tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.provides.build: tests/CMakeFiles/RStest.dir/performancetest.cpp.obj


tests/CMakeFiles/RStest.dir/rstest.cpp.obj: tests/CMakeFiles/RStest.dir/flags.make
tests/CMakeFiles/RStest.dir/rstest.cpp.obj: tests/CMakeFiles/RStest.dir/includes_CXX.rsp
tests/CMakeFiles/RStest.dir/rstest.cpp.obj: ../tests/rstest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/RStest.dir/rstest.cpp.obj"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\RStest.dir\rstest.cpp.obj -c D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\rstest.cpp

tests/CMakeFiles/RStest.dir/rstest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RStest.dir/rstest.cpp.i"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\rstest.cpp > CMakeFiles\RStest.dir\rstest.cpp.i

tests/CMakeFiles/RStest.dir/rstest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RStest.dir/rstest.cpp.s"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\rstest.cpp -o CMakeFiles\RStest.dir\rstest.cpp.s

tests/CMakeFiles/RStest.dir/rstest.cpp.obj.requires:

.PHONY : tests/CMakeFiles/RStest.dir/rstest.cpp.obj.requires

tests/CMakeFiles/RStest.dir/rstest.cpp.obj.provides: tests/CMakeFiles/RStest.dir/rstest.cpp.obj.requires
	$(MAKE) -f tests\CMakeFiles\RStest.dir\build.make tests/CMakeFiles/RStest.dir/rstest.cpp.obj.provides.build
.PHONY : tests/CMakeFiles/RStest.dir/rstest.cpp.obj.provides

tests/CMakeFiles/RStest.dir/rstest.cpp.obj.provides.build: tests/CMakeFiles/RStest.dir/rstest.cpp.obj


tests/CMakeFiles/RStest.dir/tests.cpp.obj: tests/CMakeFiles/RStest.dir/flags.make
tests/CMakeFiles/RStest.dir/tests.cpp.obj: tests/CMakeFiles/RStest.dir/includes_CXX.rsp
tests/CMakeFiles/RStest.dir/tests.cpp.obj: ../tests/tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/RStest.dir/tests.cpp.obj"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\RStest.dir\tests.cpp.obj -c D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\tests.cpp

tests/CMakeFiles/RStest.dir/tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RStest.dir/tests.cpp.i"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\tests.cpp > CMakeFiles\RStest.dir\tests.cpp.i

tests/CMakeFiles/RStest.dir/tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RStest.dir/tests.cpp.s"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests\tests.cpp -o CMakeFiles\RStest.dir\tests.cpp.s

tests/CMakeFiles/RStest.dir/tests.cpp.obj.requires:

.PHONY : tests/CMakeFiles/RStest.dir/tests.cpp.obj.requires

tests/CMakeFiles/RStest.dir/tests.cpp.obj.provides: tests/CMakeFiles/RStest.dir/tests.cpp.obj.requires
	$(MAKE) -f tests\CMakeFiles\RStest.dir\build.make tests/CMakeFiles/RStest.dir/tests.cpp.obj.provides.build
.PHONY : tests/CMakeFiles/RStest.dir/tests.cpp.obj.provides

tests/CMakeFiles/RStest.dir/tests.cpp.obj.provides.build: tests/CMakeFiles/RStest.dir/tests.cpp.obj


# Object files for target RStest
RStest_OBJECTS = \
"CMakeFiles/RStest.dir/gftest.cpp.obj" \
"CMakeFiles/RStest.dir/performancetest.cpp.obj" \
"CMakeFiles/RStest.dir/rstest.cpp.obj" \
"CMakeFiles/RStest.dir/tests.cpp.obj"

# External object files for target RStest
RStest_EXTERNAL_OBJECTS =

tests/RStest.exe: tests/CMakeFiles/RStest.dir/gftest.cpp.obj
tests/RStest.exe: tests/CMakeFiles/RStest.dir/performancetest.cpp.obj
tests/RStest.exe: tests/CMakeFiles/RStest.dir/rstest.cpp.obj
tests/RStest.exe: tests/CMakeFiles/RStest.dir/tests.cpp.obj
tests/RStest.exe: tests/CMakeFiles/RStest.dir/build.make
tests/RStest.exe: tests/CMakeFiles/RStest.dir/linklibs.rsp
tests/RStest.exe: tests/CMakeFiles/RStest.dir/objects1.rsp
tests/RStest.exe: tests/CMakeFiles/RStest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable RStest.exe"
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\RStest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/RStest.dir/build: tests/RStest.exe

.PHONY : tests/CMakeFiles/RStest.dir/build

tests/CMakeFiles/RStest.dir/requires: tests/CMakeFiles/RStest.dir/gftest.cpp.obj.requires
tests/CMakeFiles/RStest.dir/requires: tests/CMakeFiles/RStest.dir/performancetest.cpp.obj.requires
tests/CMakeFiles/RStest.dir/requires: tests/CMakeFiles/RStest.dir/rstest.cpp.obj.requires
tests/CMakeFiles/RStest.dir/requires: tests/CMakeFiles/RStest.dir/tests.cpp.obj.requires

.PHONY : tests/CMakeFiles/RStest.dir/requires

tests/CMakeFiles/RStest.dir/clean:
	cd /d D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests && $(CMAKE_COMMAND) -P CMakeFiles\RStest.dir\cmake_clean.cmake
.PHONY : tests/CMakeFiles/RStest.dir/clean

tests/CMakeFiles/RStest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\tests D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests D:\Courses\STU_courses\lab\screen_capture_and_encrypt\Reed-Solomon-master\cmake-build-debug\tests\CMakeFiles\RStest.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/RStest.dir/depend
