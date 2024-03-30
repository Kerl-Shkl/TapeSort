#include "TapeInsertSort.h"
#include <algorithm>
#include <stdexcept>

namespace tape {

TapeInsertSort::TapeInsertSort(TapePtr source, TapePtr destination,
                               size_t memoryCap)
: source(std::move(source))
, destination(std::move(destination))
, memoryCap(memoryCap)
{}

void TapeInsertSort::sort()
{
    source->rewind();
    destination->clear();
    do {
        fillBuffer();
        dumpBuffer();
        updatePassed();
    } while (!memoryBuffer.empty());
    destination->rewind();
}

void TapeInsertSort::fillBuffer()
{
    source->rewind();
    memoryBuffer.clear();
    passedCount = 0;
    uint32_t number;
    while (source->read(number)) {
        checkValue(number);
    }
    std::sort_heap(memoryBuffer.begin(), memoryBuffer.end());
}

void TapeInsertSort::checkValue(uint32_t number)
{
    if (bufferFull() && number > memoryBuffer[0]) {
        return;
    }
    if (number > bottomLine) {
        insertInHeap(number);
    }
    else if (number == bottomLine) {
        if (passedCount >= toPassCount) {
            insertInHeap(number);
        }
        ++passedCount;
    }
}

void TapeInsertSort::updatePassed()
{
    if (memoryBuffer.empty()) {
        toPassCount = 0;
        bottomLine = 0;
        return;
    }
    auto newPassed = countMaxInSortedBuffer();
    if (memoryBuffer.back() == bottomLine) {
        toPassCount += newPassed;
    }
    else {
        toPassCount = newPassed;
        bottomLine = memoryBuffer.back();
    }
}

void TapeInsertSort::dumpBuffer()
{
    for (uint32_t value : memoryBuffer) {
        bool writed = destination->write(value);
        if (!writed) {
            throw std::runtime_error("Can't write value in tape");
        }
    }
}

void TapeInsertSort::insertInHeap(uint32_t number)
{
    if (bufferFull()) {
        std::pop_heap(memoryBuffer.begin(), memoryBuffer.end());
        memoryBuffer.back() = number;
    }
    else {
        memoryBuffer.push_back(number);
    }
    std::push_heap(memoryBuffer.begin(), memoryBuffer.end());
}

bool TapeInsertSort::bufferFull() const
{
    return memoryBuffer.size() >= memoryCap;
}

uint32_t TapeInsertSort::countMaxInSortedBuffer()
{
    const auto from = std::lower_bound(memoryBuffer.begin(), memoryBuffer.end(),
                                       memoryBuffer.back());
    return std::distance(from, memoryBuffer.end());
}

}  // namespace tape
