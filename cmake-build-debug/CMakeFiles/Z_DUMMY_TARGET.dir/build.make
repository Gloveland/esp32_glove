# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Z_DUMMY_TARGET.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Z_DUMMY_TARGET.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Z_DUMMY_TARGET.dir/flags.make

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj: ../lib/Adxl/Adxl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj -c /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Adxl/Adxl.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.i"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Adxl/Adxl.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.s"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Adxl/Adxl.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj: ../lib/Ble/Ble.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj -c /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Ble/Ble.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.i"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Ble/Ble.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.s"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Ble/Ble.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj: ../lib/Mpu/Mpu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj -c /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Mpu/Mpu.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.i"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Mpu/Mpu.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.s"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/lib/Mpu/Mpu.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.s

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj: CMakeFiles/Z_DUMMY_TARGET.dir/flags.make
CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj -c /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/src/main.cpp

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/src/main.cpp > CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.i

CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s"
	/Users/jazminferreiro/.platformio/packages/toolchain-xtensa32@2.50200.97/bin/xtensa-esp32-elf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/src/main.cpp -o CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.s

# Object files for target Z_DUMMY_TARGET
Z_DUMMY_TARGET_OBJECTS = \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj" \
"CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj"

# External object files for target Z_DUMMY_TARGET
Z_DUMMY_TARGET_EXTERNAL_OBJECTS =

Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/Adxl/Adxl.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/Ble/Ble.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/lib/Mpu/Mpu.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/src/main.cpp.obj
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/build.make
Z_DUMMY_TARGET: CMakeFiles/Z_DUMMY_TARGET.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Z_DUMMY_TARGET"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Z_DUMMY_TARGET.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Z_DUMMY_TARGET.dir/build: Z_DUMMY_TARGET

.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/build

CMakeFiles/Z_DUMMY_TARGET.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Z_DUMMY_TARGET.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/clean

CMakeFiles/Z_DUMMY_TARGET.dir/depend:
	cd /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug /Users/jazminferreiro/Documents/PlatformIO/Projects/esp32_glove/cmake-build-debug/CMakeFiles/Z_DUMMY_TARGET.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Z_DUMMY_TARGET.dir/depend
