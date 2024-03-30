#ifndef TAPESORTALGORITHM_INCLUDE_TAPEMERGESORT_H
#define TAPESORTALGORITHM_INCLUDE_TAPEMERGESORT_H

#include "ITape.h"
#include <memory>
#include <vector>

namespace tape {

class TapeMergeSort
{
public:
    using TapePtr = std::shared_ptr<ITape>;

    TapeMergeSort(TapePtr source, TapePtr destination, size_t memoryCap);
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
    void dumpBufferIfNeeded();
    void dumpBufferToTape(TapePtr& tape);
    void fillDestination();
    void fillAnotherIntermTape();
    bool needMerge() const;
    void merge();
    void mergeToDestination();
    void mergeToInterm();
    void addMemoryBufferToFilledIntermTapes();
    void clearAfterMergeToInterm();
    void clearAfterMergeToDestination();
    void clearFilledIntermTapes();

    void printState();

    TapePtr source;
    TapePtr destination;
    std::vector<TapePtr> emptyIntermTapes;
    std::vector<TapePtr> filledIntermTapes;
    std::vector<uint32_t> memoryBuffer;
    const size_t memoryCapacity;
    bool destinationBusy = false;
    bool sourceEnds = false;
};

}  // namespace tape

#endif  // end of include guard: TAPESORTALGORITHM_INCLUDE_TAPEMERGESORT_H
