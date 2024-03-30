#include "TapeMergeSort.h"
#include "Merge.h"
#include "TapeFunctoins.h"
#include "VectorTape.h"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace tape {

TapeMergeSort::TapeMergeSort(TapePtr source, TapePtr destination,
                             size_t memoryCap)
: source(std::move(source))
, destination(std::move(destination))
, memoryBuffer(memoryCap)
, memoryCapacity(memoryCap)
{}

void TapeMergeSort::addIntermTape(TapePtr tape)
{
    tape->clear();
    emptyIntermTapes.emplace_back(std::move(tape));
}

void TapeMergeSort::sort()
{
    source->rewind();
    destination->clear();
    allocateMemoryBuffer();
    while (!sourceEnds) {
        fillBuffer();
        sortBuffer();
        dumpBufferIfNeeded();
    }
    fillDestination();
}

void TapeMergeSort::allocateMemoryBuffer()
{
    memoryBuffer.resize(memoryCapacity);
}

void TapeMergeSort::fillBuffer()
{
    for (size_t i = 0; i < memoryBuffer.size(); ++i) {
        uint32_t value;
        if (source->read(value)) {
            memoryBuffer[i] = value;
        }
        else {
            memoryBuffer.resize(i);
            sourceEnds = true;
            break;
        }
    }
}

void TapeMergeSort::sortBuffer()
{
    std::sort(memoryBuffer.begin(), memoryBuffer.end());
}

void TapeMergeSort::dumpBufferIfNeeded()
{
    if (sourceEnds) {
        return;
    }
    if (needMerge()) {
        merge();
    }
    else {
        fillAnotherIntermTape();
    }
}

void TapeMergeSort::merge()
{
    if (destinationBusy) {
        mergeToInterm();
    }
    else {
        mergeToDestination();
    }
}

void TapeMergeSort::dumpBufferToTape(TapePtr& tape)
{
    for (uint32_t value : memoryBuffer) {
        bool writed = tape->write(value);
        if (!writed) {
            throw std::runtime_error("Can't write value in tape");
        }
    }
}

void TapeMergeSort::fillDestination()
{
    if (!destinationBusy) {
        mergeToDestination();
    }
    else {
        mergeToInterm();
        copyTape(*destination, *filledIntermTapes.front());
        destination->rewind();
    }
}

void TapeMergeSort::fillAnotherIntermTape()
{
    if (emptyIntermTapes.size() > 1) {
        auto& anotherTape = emptyIntermTapes.back();
        dumpBufferToTape(anotherTape);
        filledIntermTapes.emplace_back(std::move(anotherTape));
        emptyIntermTapes.pop_back();
    }
    else {
        dumpBufferToTape(destination);
        destinationBusy = true;
        filledIntermTapes.push_back(destination);
    }
}

bool TapeMergeSort::needMerge() const
{
    assert((destinationBusy && !emptyIntermTapes.empty()) || !destinationBusy);
    return (destinationBusy && emptyIntermTapes.size() == 1) ||
           emptyIntermTapes.empty();
}

void TapeMergeSort::mergeToDestination()
{
    assert(!destinationBusy);
    addMemoryBufferToFilledIntermTapes();
    MergeTapes merger(destination, filledIntermTapes);
    merger.merge();
    clearAfterMergeToDestination();
}

void TapeMergeSort::mergeToInterm()
{
    assert(emptyIntermTapes.size() == 1 && destinationBusy);
    // filledIntermTapes.push_back(destination);
    addMemoryBufferToFilledIntermTapes();
    MergeTapes merger(emptyIntermTapes.front(), filledIntermTapes);
    merger.merge();
    clearAfterMergeToInterm();
}

void TapeMergeSort::addMemoryBufferToFilledIntermTapes()
{
    auto memTapePtr =
        std::shared_ptr<VectorTape>(new VectorTape, [this](ITape *tape) {
            VectorTape *vecTape = dynamic_cast<VectorTape *>(tape);
            memoryBuffer = std::move(vecTape->getVector());
            delete tape;
        });
    memTapePtr->setVector(std::move(memoryBuffer));
    filledIntermTapes.push_back(memTapePtr);
}

void TapeMergeSort::clearAfterMergeToInterm()
{
    assert(emptyIntermTapes.size() == 1 && destinationBusy);
    filledIntermTapes.pop_back();  // release memory buffer
    filledIntermTapes.pop_back();  // release destination
    destination->clear();
    destinationBusy = false;

    TapePtr receiver = std::move(emptyIntermTapes.front());
    emptyIntermTapes.pop_back();
    receiver->rewind();

    clearFilledIntermTapes();

    filledIntermTapes.emplace_back(std::move(receiver));
}

void TapeMergeSort::clearAfterMergeToDestination()
{
    destination->rewind();
    destinationBusy = true;
    filledIntermTapes.pop_back();  // release memoryBuffer
    clearFilledIntermTapes();
    filledIntermTapes.push_back(destination);
}

void TapeMergeSort::clearFilledIntermTapes()
{
    for (auto& intermTape : filledIntermTapes) {
        intermTape->clear();
        emptyIntermTapes.emplace_back(std::move(intermTape));
    }
    filledIntermTapes.clear();
}

}  // namespace tape
