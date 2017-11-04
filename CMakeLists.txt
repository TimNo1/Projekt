cmake_minimum_required (VERSION 2.6)
project (Projekt)
# The version number.
set (Projekt_VERSION_MAJOR 0)
set (Projekt_VERSION_MINOR 1)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ProjektConfig.h.in"
  "${PROJECT_BINARY_DIR}/ProjektConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add directories for libraries
include_directories ("${PROJECT_SOURCE_DIR}/KmerFinder")
include_directories ("${PROJECT_SOURCE_DIR}/fasta")
add_subdirectory (KmerFinder)
add_subdirectory (fasta)

# add the executable
add_executable(Projekt main.cpp)
target_link_libraries (Projekt KmerFinder)
target_link_libraries (Projekt fasta)

# add a "test" target:
enable_testing()

# does the application run
# add_test (ProjektRuns Projekt argument)
