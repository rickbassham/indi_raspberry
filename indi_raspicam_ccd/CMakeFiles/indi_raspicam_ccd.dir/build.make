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
CMAKE_SOURCE_DIR = /home/pi/indi_raspberry/indi_raspicam_ccd

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/indi_raspberry/indi_raspicam_ccd

# Include any dependencies generated for this target.
include CMakeFiles/indi_raspicam_ccd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/indi_raspicam_ccd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/indi_raspicam_ccd.dir/flags.make

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o: CMakeFiles/indi_raspicam_ccd.dir/flags.make
CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o: raspicam_ccd.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/indi_raspberry/indi_raspicam_ccd/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o -c /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_ccd.cpp

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_ccd.cpp > CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.i

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_ccd.cpp -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.s

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.requires:
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.requires

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.provides: CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.requires
	$(MAKE) -f CMakeFiles/indi_raspicam_ccd.dir/build.make CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.provides.build
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.provides

CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.provides.build: CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o: CMakeFiles/indi_raspicam_ccd.dir/flags.make
CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o: raspicam_driver.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/indi_raspberry/indi_raspicam_ccd/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o   -c /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_driver.c

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_driver.c > CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.i

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_driver.c -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.s

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.requires:
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.requires

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.provides: CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.requires
	$(MAKE) -f CMakeFiles/indi_raspicam_ccd.dir/build.make CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.provides.build
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.provides

CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.provides.build: CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o: CMakeFiles/indi_raspicam_ccd.dir/flags.make
CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o: raspicam_readimage.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/indi_raspberry/indi_raspicam_ccd/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o   -c /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_readimage.c

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_readimage.c > CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.i

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/pi/indi_raspberry/indi_raspicam_ccd/raspicam_readimage.c -o CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.s

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.requires:
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.requires

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.provides: CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.requires
	$(MAKE) -f CMakeFiles/indi_raspicam_ccd.dir/build.make CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.provides.build
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.provides

CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.provides.build: CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o

# Object files for target indi_raspicam_ccd
indi_raspicam_ccd_OBJECTS = \
"CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o" \
"CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o" \
"CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o"

# External object files for target indi_raspicam_ccd
indi_raspicam_ccd_EXTERNAL_OBJECTS =

indi_raspicam_ccd: CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o
indi_raspicam_ccd: CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o
indi_raspicam_ccd: CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o
indi_raspicam_ccd: CMakeFiles/indi_raspicam_ccd.dir/build.make
indi_raspicam_ccd: /usr/local/lib/libindidriver.so
indi_raspicam_ccd: /usr/lib/arm-linux-gnueabihf/libcfitsio.so
indi_raspicam_ccd: /usr/lib/arm-linux-gnueabihf/libjpeg.so
indi_raspicam_ccd: /usr/lib/arm-linux-gnueabihf/libz.so
indi_raspicam_ccd: CMakeFiles/indi_raspicam_ccd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable indi_raspicam_ccd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/indi_raspicam_ccd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/indi_raspicam_ccd.dir/build: indi_raspicam_ccd
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/build

CMakeFiles/indi_raspicam_ccd.dir/requires: CMakeFiles/indi_raspicam_ccd.dir/raspicam_ccd.o.requires
CMakeFiles/indi_raspicam_ccd.dir/requires: CMakeFiles/indi_raspicam_ccd.dir/raspicam_driver.o.requires
CMakeFiles/indi_raspicam_ccd.dir/requires: CMakeFiles/indi_raspicam_ccd.dir/raspicam_readimage.o.requires
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/requires

CMakeFiles/indi_raspicam_ccd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/indi_raspicam_ccd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/clean

CMakeFiles/indi_raspicam_ccd.dir/depend:
	cd /home/pi/indi_raspberry/indi_raspicam_ccd && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/indi_raspberry/indi_raspicam_ccd /home/pi/indi_raspberry/indi_raspicam_ccd /home/pi/indi_raspberry/indi_raspicam_ccd /home/pi/indi_raspberry/indi_raspicam_ccd /home/pi/indi_raspberry/indi_raspicam_ccd/CMakeFiles/indi_raspicam_ccd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/indi_raspicam_ccd.dir/depend

