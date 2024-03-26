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
    for (size_t i = 0; i < numbers.size(); ++i) {
        uint32_t number;
        if (full.read(number)) {
            full.stepForward();
            ASSERT_EQ(number, numbers[i]);
        }
        else {
            break;
        }
    }
}

TEST_F(VectorTapeTest, errorReadFromEmptyFile)
{
    uint32_t number;
    EXPECT_FALSE(empty.read(number));
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
        empty.stepForward();
    }
    empty.rewind();
    for (size_t i = 0; i < numbers.size(); ++i) {
        uint32_t number;
        if (full.read(number)) {
            full.stepForward();
            ASSERT_EQ(numbers[i], number);
        }
        else {
            break;
        }
    }
}
