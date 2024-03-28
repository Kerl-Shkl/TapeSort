#include "TapeFunctoins.h"

namespace tape {

bool copyTape(ITape& destination, ITape& source)
{
    destination.clear();
    source.rewind();
    uint32_t value;
    while (source.read(value)) {
        bool writed = destination.write(value);
        if (!writed) {
            return false;
        }
    }
    return true;
}

}  // namespace tape
