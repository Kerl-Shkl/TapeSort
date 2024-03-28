#include "TapeSort.h"
#include "Merge.h"
#include "TapeFunctoins.h"
#include "VectorTape.h"
#include <algorithm>
#include <cassert>

TapeSort::TapeSort(TapePtr source, TapePtr destination, size_t memoryCap)
: source(std::move(source))
, destination(std::move(destination))
, memoryBuffer(memoryCap)
, memoryCapacity(memoryCap)
{}

void TapeSort::addIntermTape(TapePtr tape)
{
    tape->clear();
    emptyIntermTapes.emplace_back(std::move(tape));
}

void TapeSort::sort()
{
    allocateMemoryBuffer();
    while (!sourceEnds) {
        fillBuffer();
        sortBuffer();
        dumpBufferIfNeeded();
    }
    fillDestination();
}

void TapeSort::allocateMemoryBuffer()
{
    memoryBuffer.resize(memoryCapacity);
}

void TapeSort::fillBuffer()
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

void TapeSort::sortBuffer()
{
    std::sort(memoryBuffer.begin(), memoryBuffer.end());
}

void TapeSort::dumpBufferIfNeeded()
{
    if (sourceEnds) {
        return;
    }
    if (needMerge()) {
        mergeToInterm();
    }
    else {
        fillAnotherIntermTape();
    }
}

void TapeSort::dumpBufferToTape(TapePtr& tape)
{
    for (uint32_t value : memoryBuffer) {
        tape->write(value);  // TODO check success
    }
}

void TapeSort::fillDestination()
{
    if (!destinationBusy) {
        mergeToDestination();
    }
    else {
        mergeToInterm();
        tape::copyTape(*destination, *filledIntermTapes.front());
    }
}

void TapeSort::fillAnotherIntermTape()
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

bool TapeSort::needMerge() const
{
    return destinationBusy && emptyIntermTapes.size() == 1;
}

void TapeSort::mergeToDestination()
{
    assert(!destinationBusy);
    addMemoryBufferToFilledIntermTapes();
    MergeTapes merger(destination, filledIntermTapes);
    merger.merge();
    destination->rewind();
    filledIntermTapes.pop_back();  // release memoryBuffer
}

void TapeSort::mergeToInterm()
{
    assert(emptyIntermTapes.size() == 1 && destinationBusy);
    filledIntermTapes.push_back(destination);
    addMemoryBufferToFilledIntermTapes();
    MergeTapes merger(emptyIntermTapes.front(), filledIntermTapes);
    merger.merge();
    clearAfterMergeToInterm();
}

void TapeSort::addMemoryBufferToFilledIntermTapes()
{
    auto memTapePtr = std::shared_ptr<tape::VectorTape>(
        new tape::VectorTape, [this](tape::ITape *tape) {
            tape::VectorTape *vecTape = dynamic_cast<tape::VectorTape *>(tape);
            memoryBuffer = std::move(vecTape->getVector());
            delete tape;
        });
    memTapePtr->setVector(std::move(memoryBuffer));
    filledIntermTapes.push_back(memTapePtr);
}

void TapeSort::clearAfterMergeToInterm()
{
    assert(emptyIntermTapes.size() == 1 && destinationBusy);
    filledIntermTapes.pop_back();  // release memory buffer
    filledIntermTapes.pop_back();  // release destination
    destination->clear();
    destinationBusy = false;

    TapePtr receiver = std::move(emptyIntermTapes.front());
    emptyIntermTapes.pop_back();
    receiver->rewind();

    for (auto& intermTape : filledIntermTapes) {
        intermTape->clear();
        emptyIntermTapes.emplace_back(std::move(intermTape));
    }
    filledIntermTapes.clear();

    filledIntermTapes.emplace_back(std::move(receiver));
}
