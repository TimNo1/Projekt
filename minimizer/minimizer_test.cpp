//
// Created by dario on 07.12.17..
//
#include <gtest/gtest.h>
#include "minimizer.h"

//isprobavanje testova
TEST(minimizerTest, minimizerTest) {
    std::vector<minimizer::MinimizerTriple> result = minimizer::computeMinimizers("ACTG", 1, 3);
    ASSERT_EQ(2, (int)result.size());
//    ASSERT_EQ(1, 2);
}

