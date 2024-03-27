#include "TapeSort.h"
#include "VectorTape.h"
#include <gtest/gtest.h>

class PublicMergeTapeSort : public TapeSort
{
public:
    PublicMergeTapeSort(TapePtr source, TapePtr destination, size_t memoryCap)
    : TapeSort(source, destination, memoryCap)
    {}
    using TapeSort::destinationBusy;
    using TapeSort::emptyIntermTapes;
    using TapeSort::filledIntermTapes;
    using TapeSort::memoryBuffer;
    using TapeSort::mergeToDestination;
    using TapeSort::mergeToInterm;
};

class TapeSortTestMergeToInterm : public testing::Test
{
protected:
    using TapePtr = std::shared_ptr<ITape>;

    TapeSortTestMergeToInterm()
    : destination(std::make_shared<VectorTape>())
    , sorter(source, destination, 2)
    {}

    void SetUp() override
    {
        auto filledInterm = std::make_shared<VectorTape>();
        filledInterm->setVector({1, 11});
        sorter.filledIntermTapes.emplace_back(std::move(filledInterm));
        destination->write(3);
        destination->stepForward();
        destination->write(13);
        destination->stepForward();
        sorter.destinationBusy = true;
        sorter.memoryBuffer = {2, 12};
        sorter.emptyIntermTapes.emplace_back(std::make_shared<VectorTape>());
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
    auto dest = sorter.filledIntermTapes.front();
    uint32_t value;
    for (size_t i = 0; i < expected.size(); ++i) {
        bool readed = dest->read(value);
        ASSERT_TRUE(readed);
        ASSERT_EQ(value, expected[i]);
        dest->stepForward();
    }
    EXPECT_FALSE(dest->read(value));
    EXPECT_EQ(sorter.memoryBuffer.size(), 2);
    EXPECT_EQ(sorter.memoryBuffer.front(), 2);
    EXPECT_EQ(sorter.memoryBuffer.back(), 12);
    EXPECT_EQ(sorter.emptyIntermTapes.size(), 1);
    EXPECT_FALSE(sorter.destinationBusy);
}
