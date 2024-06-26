#include "TapeMergeSort.h"
#include "VectorTape.h"
#include <gtest/gtest.h>

class PublicMergeTapeSort : public tape::TapeMergeSort
{
public:
    PublicMergeTapeSort(TapePtr source, TapePtr destination, size_t memoryCap)
    : TapeMergeSort(source, destination, memoryCap)
    {}
    using TapeMergeSort::destinationBusy;
    using TapeMergeSort::emptyIntermTapes;
    using TapeMergeSort::filledIntermTapes;
    using TapeMergeSort::memoryBuffer;
    using TapeMergeSort::mergeToDestination;
    using TapeMergeSort::mergeToInterm;
};

class TapeSortTestMergeToInterm : public testing::Test
{
protected:
    using TapePtr = std::shared_ptr<tape::ITape>;

    TapeSortTestMergeToInterm()
    : destination(std::make_shared<tape::VectorTape>())
    , sorter(source, destination, 2)
    {}

    void SetUp() override
    {
        auto filledInterm = std::make_shared<tape::VectorTape>();
        filledInterm->setVector({1, 11});
        sorter.filledIntermTapes.emplace_back(std::move(filledInterm));
        destination->write(3);
        destination->write(13);
        sorter.destinationBusy = true;
        sorter.filledIntermTapes.push_back(destination);
        sorter.memoryBuffer = {2, 12};
        sorter.emptyIntermTapes.emplace_back(
            std::make_shared<tape::VectorTape>());
    }

    TapePtr source = nullptr;
    TapePtr destination;
    PublicMergeTapeSort sorter;
};

TEST_F(TapeSortTestMergeToInterm, mergeToInterm)
{
    std::vector<uint32_t> expected = {1, 2, 3, 11, 12, 13};
    sorter.mergeToInterm();

    ASSERT_EQ(sorter.filledIntermTapes.size(), 1);
    auto result = sorter.filledIntermTapes.front();
    uint32_t value;
    for (size_t i = 0; i < expected.size(); ++i) {
        bool readed = result->read(value);
        ASSERT_TRUE(readed);
        ASSERT_EQ(value, expected[i]);
    }
    EXPECT_FALSE(result->peek(value));
    EXPECT_EQ(sorter.memoryBuffer.size(), 2);
    EXPECT_EQ(sorter.memoryBuffer.front(), 2);
    EXPECT_EQ(sorter.memoryBuffer.back(), 12);
    EXPECT_EQ(sorter.emptyIntermTapes.size(), 1);
    EXPECT_FALSE(sorter.destinationBusy);
}
