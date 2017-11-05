# add libraries
add_library(minimizer minimizer.h minimizer.cpp DnaSequence.h DnaSequence.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")