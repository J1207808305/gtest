#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>

using namespace testing;

class Test {
public:
    int getInterValue(void);
    int getOneInterValue(int a);
};

class MockTest {
public:
    MOCK_METHOD0(getInterValue, int());
    MOCK_METHOD1(getOneInterValue, int(int a));
};

TEST(GtestMocktest, getInterValue)
{
    MockTest test;
    int value = 10;
    EXPECT_CALL(test, getInterValue()).WillOnce(Return(value));
    EXPECT_CALL(test, getOneInterValue(10)).WillOnce(Return(value));
    EXPECT_EQ(10, test.getInterValue());
    EXPECT_EQ(10, test.getOneInterValue(10));
}

TEST(GtestMockTest, getOneInterValue)
{
    MockTest test;
    int value = 10;
    EXPECT_CALL(test, getOneInterValue(10)).WillOnce(Return(value));
    EXPECT_EQ(10, test.getOneInterValue(10));
}

// Run all the tests that were declared with TEST()
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
