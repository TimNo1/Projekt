add_library(thread_pool thread_pool.cpp thread_pool.hpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -lpthread")