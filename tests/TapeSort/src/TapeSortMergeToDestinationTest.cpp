#include "TapeMergeSort.h"
#include "VectorTape.h"
#include <gtest/gtest.h>

class PublicMergeTapeSort : public tape::TapeMergeSort
{
public:
    PublicMergeTapeSort(TapePtr source, TapePtr destination, size_t memoryCap)
    : TapeMergeSort(source, destination, memoryCap)
    {}
    using TapeMergeSort::filledIntermTapes;
    using TapeMergeSort::memoryBuffer;
    using TapeMergeSort::mergeToDestination;
    using TapeMergeSort::mergeToInterm;
};

class TapeSortTestMergeToDestination : public testing::Test
{
protected:
    using TapePtr = std::shared_ptr<tape::ITape>;

    TapeSortTestMergeToDestination()
    : destination(std::make_shared<tape::VectorTape>())
    , sorter(source, destination, 2)
    {}

    void SetUp() override
    {
        auto filledInterm = std::make_shared<tape::VectorTape>();
        filledInterm->setVector({1, 11});
        sorter.filledIntermTapes.emplace_back(std::move(filledInterm));
        sorter.memoryBuffer = {2, 12};
    }

    TapePtr source = nullptr;
    TapePtr destination;
    PublicMergeTapeSort sorter;
};

TEST_F(TapeSortTestMergeToDestination, mergeToDestination)
{
    std::vector<uint32_t> expected = {1, 2, 11, 12};
    sorter.mergeToDestination();

    uint32_t value;
    for (size_t i = 0; i < expected.size(); ++i) {
        bool readed = destination->read(value);
        ASSERT_TRUE(readed);
        ASSERT_EQ(value, expected[i]);
    }
    EXPECT_FALSE(destination->peek(value));
    EXPECT_EQ(sorter.memoryBuffer.size(), 2);
    EXPECT_EQ(sorter.memoryBuffer.front(), 2);
    EXPECT_EQ(sorter.memoryBuffer.back(), 12);
}
