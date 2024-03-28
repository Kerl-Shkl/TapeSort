#include "VectorTape.h"
#include <gtest/gtest.h>

class VectorTapeTest : public testing::Test
{
protected:
    void SetUp() override
    {
        full.setVector(numbers);
    }

    std::vector<uint32_t> numbers = {13, 15, 21, 81, 92};
    VectorTape full;
    VectorTape empty;
};

TEST_F(VectorTapeTest, successReadFromFullTape)
{
    uint32_t number;
    for (size_t i = 0; i < numbers.size(); ++i) {
        bool readed = full.read(number);
        ASSERT_TRUE(readed);
        ASSERT_EQ(number, numbers[i]);
    }
    EXPECT_FALSE(full.peek(number));
}

TEST_F(VectorTapeTest, errorReadFromEmpty)
{
    uint32_t number;
    EXPECT_FALSE(empty.peek(number));
}

TEST_F(VectorTapeTest, errorOnStepBeforeBegin)
{
    full.rewind();
    EXPECT_FALSE(full.stepBackward());
}

TEST_F(VectorTapeTest, writeRewindRead)
{
    for (size_t i = 0; i < numbers.size(); ++i) {
        empty.write(numbers[i]);
    }
    empty.rewind();

    uint32_t number;
    for (size_t i = 0; i < numbers.size(); ++i) {
        bool readed = empty.read(number);
        ASSERT_TRUE(readed);
        ASSERT_EQ(numbers[i], number);
    }
    EXPECT_FALSE(empty.peek(number));
}
