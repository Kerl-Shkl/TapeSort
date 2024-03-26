#ifndef TAPE_INCLUDE_ITAPE_H
#define TAPE_INCLUDE_ITAPE_H

#include <cstdint>

class ITape
{
public:
    virtual ~ITape() = default;
    virtual bool read(uint32_t& buffer) = 0;
    virtual bool write(uint32_t value) = 0;
    virtual bool stepForward() = 0;
    virtual bool stepBackward() = 0;
    virtual bool rewind() = 0;
};

#endif  // end of include guard: TAPE_INCLUDE_ITAPE_H
