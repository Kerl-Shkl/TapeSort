#include "VectorTape.h"

bool VectorTape::read(uint32_t& recipient)
{
    bool peekSuccess = peek(recipient);
    if (peekSuccess) {
        stepForward();
    }
    return peekSuccess;
}

bool VectorTape::write(uint32_t value)
{
    bool putSuccess = put(value);
    if (putSuccess) {
        stepForward();
    }
    return putSuccess;
}

bool VectorTape::peek(uint32_t& recipient)
{
    if (pos >= actualEnd) {
        return false;
    }
    recipient = buffer[pos];
    return true;
}

bool VectorTape::put(uint32_t value)
{
    resizeIfNeeded();
    if (pos >= actualEnd) {
        actualEnd = pos + 1;
    }
    buffer[pos] = value;
    return true;
}

bool VectorTape::stepForward()
{
    ++pos;
    return true;
}

bool VectorTape::stepBackward()
{
    if (pos <= 0) {
        return false;
    }
    --pos;
    return true;
}

bool VectorTape::rewind()
{
    pos = 0;
    return true;
}

bool VectorTape::clear()
{
    actualEnd = 0;
    return rewind();
}

void VectorTape::setVector(std::vector<uint32_t> vec)
{
    buffer = std::move(vec);
    actualEnd = buffer.size();
}

std::vector<uint32_t>& VectorTape::getVector()
{
    return buffer;
}

const std::vector<uint32_t>& VectorTape::getVector() const
{
    return buffer;
}

void VectorTape::resizeIfNeeded()
{
    if (pos >= buffer.size()) {
        auto new_size = pos == 0 ? 1 : pos * 2;
        buffer.resize(new_size);
    }
}
