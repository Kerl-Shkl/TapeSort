#include "VectorTape.h"

bool VectorTape::read(uint32_t& recipient)
{
    if (pos >= buffer.size()) {
        return false;
    }
    recipient = buffer[pos];
    return true;
}

bool VectorTape::write(uint32_t value)
{
    resizeIfNeeded();
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

void VectorTape::setVector(std::vector<uint32_t> vec)
{
    buffer = std::move(vec);
}

void VectorTape::resizeIfNeeded()
{
    if (pos >= buffer.size()) {
        auto new_size = pos == 0 ? 5 : pos * 2;
        buffer.resize(new_size);
    }
}
