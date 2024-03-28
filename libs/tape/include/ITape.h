#ifndef TAPE_INCLUDE_ITAPE_H
#define TAPE_INCLUDE_ITAPE_H

#include <cstdint>

namespace tape {

class ITape
{
public:
    virtual ~ITape() = default;
    virtual bool read(uint32_t& buffer) = 0;
    virtual bool write(uint32_t value) = 0;
    virtual bool peek(uint32_t& buffer) = 0;
    virtual bool put(uint32_t value) = 0;
    virtual bool stepForward() = 0;
    virtual bool stepBackward() = 0;
    virtual bool rewind() = 0;
    virtual bool clear() = 0;
};

}  // namespace tape

#endif  // end of include guard: TAPE_INCLUDE_ITAPE_H
