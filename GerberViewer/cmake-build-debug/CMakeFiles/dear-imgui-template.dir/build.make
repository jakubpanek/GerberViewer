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
CMAKE_SOURCE_DIR = /Users/jakubpanek/Desktop/GerberViewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/dear-imgui-template.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dear-imgui-template.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dear-imgui-template.dir/flags.make

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o: CMakeFiles/dear-imgui-template.dir/flags.make
CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o: ../vendor/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o -c /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_glfw.cpp

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_glfw.cpp > CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.i

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_glfw.cpp -o CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.s

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o: CMakeFiles/dear-imgui-template.dir/flags.make
CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o: ../vendor/imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o -c /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_opengl3.cpp

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_opengl3.cpp > CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.i

CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jakubpanek/Desktop/GerberViewer/vendor/imgui_impl_opengl3.cpp -o CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.s

CMakeFiles/dear-imgui-template.dir/src/main.cpp.o: CMakeFiles/dear-imgui-template.dir/flags.make
CMakeFiles/dear-imgui-template.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/dear-imgui-template.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dear-imgui-template.dir/src/main.cpp.o -c /Users/jakubpanek/Desktop/GerberViewer/src/main.cpp

CMakeFiles/dear-imgui-template.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dear-imgui-template.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jakubpanek/Desktop/GerberViewer/src/main.cpp > CMakeFiles/dear-imgui-template.dir/src/main.cpp.i

CMakeFiles/dear-imgui-template.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dear-imgui-template.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jakubpanek/Desktop/GerberViewer/src/main.cpp -o CMakeFiles/dear-imgui-template.dir/src/main.cpp.s

# Object files for target dear-imgui-template
dear__imgui__template_OBJECTS = \
"CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o" \
"CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o" \
"CMakeFiles/dear-imgui-template.dir/src/main.cpp.o"

# External object files for target dear-imgui-template
dear__imgui__template_EXTERNAL_OBJECTS =

dear-imgui-template: CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_glfw.cpp.o
dear-imgui-template: CMakeFiles/dear-imgui-template.dir/vendor/imgui_impl_opengl3.cpp.o
dear-imgui-template: CMakeFiles/dear-imgui-template.dir/src/main.cpp.o
dear-imgui-template: CMakeFiles/dear-imgui-template.dir/build.make
dear-imgui-template: libglad.a
dear-imgui-template: lib/glfw/src/libglfw3.a
dear-imgui-template: libimgui.a
dear-imgui-template: CMakeFiles/dear-imgui-template.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable dear-imgui-template"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dear-imgui-template.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dear-imgui-template.dir/build: dear-imgui-template

.PHONY : CMakeFiles/dear-imgui-template.dir/build

CMakeFiles/dear-imgui-template.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dear-imgui-template.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dear-imgui-template.dir/clean

CMakeFiles/dear-imgui-template.dir/depend:
	cd /Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jakubpanek/Desktop/GerberViewer /Users/jakubpanek/Desktop/GerberViewer /Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug /Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug /Users/jakubpanek/Desktop/GerberViewer/cmake-build-debug/CMakeFiles/dear-imgui-template.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dear-imgui-template.dir/depend

