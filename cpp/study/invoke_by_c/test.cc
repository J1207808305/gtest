#include "cpp.h"
#include <gtest/gtest.h>
#include <iostream>

int call_cpp_fun(Cpp* p, int i);

TEST(CInvokeTest, cpp)
{
    struct Cpp cpp;
    EXPECT_EQ(1, call_cpp_fun(&cpp, 1));
}

// Run all the tests that were declared with TEST()
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
