#include "netsim.hpp"
#include <iostream>
#include "package.hpp"

#include "test/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}