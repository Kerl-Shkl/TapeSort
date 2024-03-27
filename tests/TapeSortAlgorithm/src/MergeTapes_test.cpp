#include "Merge.h"
#include "VectorTape.h"
#include <gtest/gtest.h>

class MergeTapesTest : public testing::Test
{
protected:
    using TapePtr = std::shared_ptr<ITape>;

    void SetUp() override
    {
        createDestination();
        createHelpTapes();
        fillHelpTapesWithNumbers();
    }

    std::vector<uint32_t> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<TapePtr> helpTapes;
    TapePtr destination;

private:
    void createDestination()
    {
        destination = std::make_shared<VectorTape>();
    }

    void createHelpTapes()
    {
        for (int i = 0; i < 4; ++i) {
            helpTapes.emplace_back(std::make_shared<VectorTape>());
        }
    }

    void fillHelpTapesWithNumbers()
    {
        std::vector<TapePtr>::size_type tapeIndex = 0;
        for (uint32_t number : numbers) {
            helpTapes[tapeIndex]->write(number);
            helpTapes[tapeIndex]->stepForward();
            tapeIndex = tapeIndex+1 >= helpTapes.size() ? 0 : tapeIndex + 1;
        }
    }
};

TEST_F(MergeTapesTest, testMerge)
{
    MergeTapes merger(destination, helpTapes);
    merger.merge();
    destination->rewind();
    for (size_t i = 0; i < numbers.size(); ++i) {
        uint32_t number;
        bool readed = destination->read(number);
        ASSERT_TRUE(readed);
        destination->stepForward();
        ASSERT_EQ(numbers[i], number);
    }
}
