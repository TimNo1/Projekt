//
// Created by dario on 07.12.17..
//
#include <gtest/gtest.h>
#include "minimizer.h"

//isprobavanje testova
TEST(minimizerTest, minimizerTest) {
    std::vector<minimizer::MinimizerTriple> result = minimizer::computeMinimizers("ACTGATGC", 2, 3);
    std::vector<minimizer::MinimizerTriple> expected = {minimizer::MinimizerTriple(9, -1, false), minimizer::MinimizerTriple(30, -1, false),
                                                        minimizer::MinimizerTriple(14, -1, true)};
    ASSERT_EQ((int)result.size(), (int)expected.size());
    for (int i = 0; i < result.size(); i++) {
        ASSERT_EQ(result[i].h, expected[i].h);
        ASSERT_EQ(result[i].rc, expected[i].rc);
    }

}

