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
CMAKE_SOURCE_DIR = /home/young/localhome/code/idSLAM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/young/localhome/code/idSLAM

# Include any dependencies generated for this target.
include src/geometry/CMakeFiles/geometry.dir/depend.make

# Include the progress variables for this target.
include src/geometry/CMakeFiles/geometry.dir/progress.make

# Include the compile flags for this target's objects.
include src/geometry/CMakeFiles/geometry.dir/flags.make

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o: src/geometry/CMakeFiles/geometry.dir/flags.make
src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o: src/geometry/PointClouds.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/young/localhome/code/idSLAM/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o"
	cd /home/young/localhome/code/idSLAM/src/geometry && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/geometry.dir/PointClouds.cpp.o -c /home/young/localhome/code/idSLAM/src/geometry/PointClouds.cpp

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geometry.dir/PointClouds.cpp.i"
	cd /home/young/localhome/code/idSLAM/src/geometry && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/young/localhome/code/idSLAM/src/geometry/PointClouds.cpp > CMakeFiles/geometry.dir/PointClouds.cpp.i

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geometry.dir/PointClouds.cpp.s"
	cd /home/young/localhome/code/idSLAM/src/geometry && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/young/localhome/code/idSLAM/src/geometry/PointClouds.cpp -o CMakeFiles/geometry.dir/PointClouds.cpp.s

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.requires:
.PHONY : src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.requires

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.provides: src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.requires
	$(MAKE) -f src/geometry/CMakeFiles/geometry.dir/build.make src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.provides.build
.PHONY : src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.provides

src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.provides.build: src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o

# Object files for target geometry
geometry_OBJECTS = \
"CMakeFiles/geometry.dir/PointClouds.cpp.o"

# External object files for target geometry
geometry_EXTERNAL_OBJECTS =

lib/libgeometry.so: src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o
lib/libgeometry.so: src/geometry/CMakeFiles/geometry.dir/build.make
lib/libgeometry.so: /usr/local/lib/libopencv_videostab.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_video.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_superres.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_stitching.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_photo.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_ocl.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_objdetect.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_nonfree.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_ml.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_legacy.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_imgproc.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_highgui.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_gpu.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_flann.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_features2d.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_core.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_contrib.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_calib3d.so.2.4.9
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libpthread.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libpthread.so
lib/libgeometry.so: /usr/lib/libpcl_common.so
lib/libgeometry.so: /usr/lib/libpcl_octree.so
lib/libgeometry.so: /usr/lib/libOpenNI.so
lib/libgeometry.so: /usr/lib/libvtkCommon.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkRendering.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkHybrid.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkCharts.so.5.8.0
lib/libgeometry.so: /usr/lib/libpcl_io.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
lib/libgeometry.so: /usr/lib/x86_64-linux-gnu/libpthread.so
lib/libgeometry.so: /usr/lib/libOpenNI.so
lib/libgeometry.so: /usr/lib/libvtkCommon.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkRendering.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkHybrid.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkCharts.so.5.8.0
lib/libgeometry.so: /usr/local/lib/libopencv_nonfree.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_ocl.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_gpu.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_photo.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_objdetect.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_legacy.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_video.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_ml.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_calib3d.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_features2d.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_highgui.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_imgproc.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_flann.so.2.4.9
lib/libgeometry.so: /usr/local/lib/libopencv_core.so.2.4.9
lib/libgeometry.so: /usr/lib/libpcl_common.so
lib/libgeometry.so: /usr/lib/libpcl_octree.so
lib/libgeometry.so: /usr/lib/libpcl_io.so
lib/libgeometry.so: /usr/lib/libvtkViews.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkInfovis.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkWidgets.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkHybrid.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkParallel.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkVolumeRendering.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkRendering.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkGraphics.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkImaging.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkIO.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkFiltering.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtkCommon.so.5.8.0
lib/libgeometry.so: /usr/lib/libvtksys.so.5.8.0
lib/libgeometry.so: src/geometry/CMakeFiles/geometry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../lib/libgeometry.so"
	cd /home/young/localhome/code/idSLAM/src/geometry && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/geometry.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/geometry/CMakeFiles/geometry.dir/build: lib/libgeometry.so
.PHONY : src/geometry/CMakeFiles/geometry.dir/build

src/geometry/CMakeFiles/geometry.dir/requires: src/geometry/CMakeFiles/geometry.dir/PointClouds.cpp.o.requires
.PHONY : src/geometry/CMakeFiles/geometry.dir/requires

src/geometry/CMakeFiles/geometry.dir/clean:
	cd /home/young/localhome/code/idSLAM/src/geometry && $(CMAKE_COMMAND) -P CMakeFiles/geometry.dir/cmake_clean.cmake
.PHONY : src/geometry/CMakeFiles/geometry.dir/clean

src/geometry/CMakeFiles/geometry.dir/depend:
	cd /home/young/localhome/code/idSLAM && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/young/localhome/code/idSLAM /home/young/localhome/code/idSLAM/src/geometry /home/young/localhome/code/idSLAM /home/young/localhome/code/idSLAM/src/geometry /home/young/localhome/code/idSLAM/src/geometry/CMakeFiles/geometry.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/geometry/CMakeFiles/geometry.dir/depend
