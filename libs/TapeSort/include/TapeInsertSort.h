#ifndef TAPESORT_INCLUDE_TAPEINSERTSORT_H
#define TAPESORT_INCLUDE_TAPEINSERTSORT_H

#include "ITape.h"
#include <memory>
#include <vector>

namespace tape {

class TapeInsertSort
{
public:
    using TapePtr = std::shared_ptr<ITape>;

    TapeInsertSort(TapePtr source, TapePtr destination, size_t memoryCap);
    void sort();

private:
    void fillBuffer();
    void updatePassed();
    void dumpBuffer();
    void checkValue(uint32_t number);
    void insertInHeap(uint32_t number);
    bool bufferFull() const;
    uint32_t countMaxInSortedBuffer();

    TapePtr source;
    TapePtr destination;
    std::vector<uint32_t> memoryBuffer;
    uint32_t memoryCap = 0;
    uint32_t bottomLine = 0;
    uint32_t toPassCount = 0;
    uint32_t passedCount = 0;
};

}  // namespace tape

#endif  // end of include guard: TAPESORT_INCLUDE_TAPEINSERTSORT_H
