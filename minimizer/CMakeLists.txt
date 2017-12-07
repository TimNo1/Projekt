# add libraries
add_library(minimizer minimizer.h minimizer.cpp DnaSequence.h DnaSequence.cpp minimizer_test.cpp)
# c++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(minimizerTest minimizer_test.cpp)
target_link_libraries(minimizerTest gtest gtest_main minimizer)
add_test(NAME minimizerTest COMMAND minimizerTest)
