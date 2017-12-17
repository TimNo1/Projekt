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

# GOOGLE TESTS CONFIG
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories("${gtest_SOURCE_DIR}/include")

# Now simply link against gtest or gtest_main as needed. Eg
# add_test(NAME example_test COMMAND example)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add directories for libraries
include_directories ("${PROJECT_SOURCE_DIR}/KmerFinder")
include_directories ("${PROJECT_SOURCE_DIR}/fasta")
include_directories ("${PROJECT_SOURCE_DIR}/minimizer")
include_directories ("${PROJECT_SOURCE_DIR}/lcsk++")
include_directories("${PROJECT_SOURCE_DIR}/lis")
include_directories("${PROJECT_SOURCE_DIR}/thread_pool")
add_subdirectory (KmerFinder)
add_subdirectory (fasta)
add_subdirectory (minimizer)
add_subdirectory (lcsk++)
add_subdirectory(lis)
add_subdirectory(thread_pool)

# add the executable
add_executable(Projekt main.cpp)
target_link_libraries (Projekt KmerFinder)
target_link_libraries (Projekt fasta)
target_link_libraries (Projekt gtest_main)
target_link_libraries (Projekt minimizer)
target_link_libraries(Projekt lcskpp)
target_link_libraries(Projekt lis)
target_link_libraries(Projekt thread_pool)

# add a "test" target:
enable_testing()

# does the application run
# add_test (ProjektRuns Projekt argument)

# c++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -Wall")
