#include "TapeInsertSort.h"
#include "VectorTape.h"
#include <gtest/gtest.h>

class TapeInsertSortTest : public testing::Test
{
protected:
    void SetUp() override
    {
        source = std::make_shared<tape::VectorTape>();
        destination = std::make_shared<tape::VectorTape>();
        fillSourceWithNumbers();
    }

    std::vector<uint32_t> numbers;
    std::shared_ptr<tape::VectorTape> source;
    std::shared_ptr<tape::VectorTape> destination;

private:
    void fillSourceWithNumbers()
    {
        srand((unsigned)time(nullptr));
        for (size_t i = 0; i < 100; ++i) {
            uint32_t number = static_cast<uint32_t>(rand() % 100);
            numbers.push_back(number);
            source->write(number);
        }
    }
};

TEST_F(TapeInsertSortTest, sortTest)
{
    tape::TapeInsertSort sorter(source, destination, 7);
    sorter.sort();
    std::sort(numbers.begin(), numbers.end());
    uint32_t value;
    for (uint32_t number : numbers) {
        bool readed = destination->read(value);
        ASSERT_TRUE(readed);
        ASSERT_EQ(value, number);
    }
    EXPECT_FALSE(destination->peek(value));
}
