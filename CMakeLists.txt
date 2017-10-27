cmake_minimum_required (VERSION 2.6)
project (Projekt)
# The version number.
set (Projekt_VERSION_MAJOR 1)
set (Projekt_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ProjektConfig.h.in"
  "${PROJECT_BINARY_DIR}/ProjektConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(Projekt main.cpp)