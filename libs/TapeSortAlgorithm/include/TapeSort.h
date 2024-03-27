#ifndef TAPESORTALGORITHM_INCLUDE_TAPESORT_H
#define TAPESORTALGORITHM_INCLUDE_TAPESORT_H

#include "ITape.h"
#include <memory>
#include <vector>

class TapeSort
{
public:
    using TapePtr = std::shared_ptr<ITape>;

    TapeSort(TapePtr source, TapePtr destination, size_t memoryCap);
    void addIntermTape(TapePtr tape);
    void sort();

#ifdef TESTS_ENABLED
protected:
#else
private:
#endif
    void allocateMemoryBuffer();
    void fillBuffer();
    void sortBuffer();
    void dumpBufferToTape(TapePtr& tape);
    void freeFilledIntermTapes();
    bool needMerge() const;
    void mergeToDestination();
    void mergeToInterm();
    void addMemoryBufferToFilledIntermTapes();
    void clearAfterMergeToInterm();

    TapePtr source;
    TapePtr destination;
    std::vector<TapePtr> emptyIntermTapes;
    std::vector<TapePtr> filledIntermTapes;
    std::vector<uint32_t> memoryBuffer;
    size_t memoryCapacity;
    bool destinationBusy = false;
    bool sourceEnds = false;
};

#endif  // end of include guard: TAPESORTALGORITHM_INCLUDE_TAPESORT_H