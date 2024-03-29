#include "TapeTimeModel.h"
#include <thread>

namespace tape {

TapeTimeModel::TapeTimeModel(std::unique_ptr<ITape> origin)
: tape(std::move(origin))
{}

bool TapeTimeModel::read(uint32_t& buffer)
{
    std::this_thread::sleep_for(readDelay);
    return tape->read(buffer);
}

bool TapeTimeModel::write(uint32_t value)
{
    std::this_thread::sleep_for(writeDelay);
    return tape->write(value);
}

bool TapeTimeModel::peek(uint32_t& buffer)
{
    std::this_thread::sleep_for(readDelay);
    return tape->peek(buffer);
}

bool TapeTimeModel::put(uint32_t value)
{
    std::this_thread::sleep_for(writeDelay);
    return tape->put(value);
}

bool TapeTimeModel::stepForward()
{
    std::this_thread::sleep_for(stepDelay);
    return tape->stepForward();
}

bool TapeTimeModel::stepBackward()
{
    std::this_thread::sleep_for(stepDelay);
    return tape->stepBackward();
}

bool TapeTimeModel::rewind()
{
    std::this_thread::sleep_for(rewindDelay);
    return tape->rewind();
}

bool TapeTimeModel::clear()
{
    std::this_thread::sleep_for(rewindDelay);
    return tape->clear();
}

void TapeTimeModel::setStepDelay(const milliseconds& delay)
{
    stepDelay = delay;
}

void TapeTimeModel::setReadDelay(const milliseconds& delay)
{
    readDelay = delay;
}

void TapeTimeModel::setWriteDelay(const milliseconds& delay)
{
    writeDelay = delay;
}

void TapeTimeModel::setRewindDelay(const milliseconds& delay)
{
    rewindDelay = delay;
}

}  // namespace tape
