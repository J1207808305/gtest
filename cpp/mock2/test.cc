#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

using namespace testing;

class Test {
public:
    int getInterValue(void);
};

class MockTest {
public:
    MOCK_METHOD0(getInterValue, int());
};

TEST(GtestMocktest, getInterValue)
{
    MockTest test;
    int value = 10;
    ON_CALL(test, getInterValue()).WillByDefault(Return(value));
    EXPECT_EQ(10, test.getInterValue());
}

// Run all the tests that were declared with TEST()
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
