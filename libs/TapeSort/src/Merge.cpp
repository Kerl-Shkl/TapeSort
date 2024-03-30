#include "Merge.h"
#include <stdexcept>

namespace tape {

MergeTapes::MergeTapes(TapePtr dest, std::vector<TapePtr>& sourceTapes)
: dest(std::move(dest))
, sourceTapes(sourceTapes)
{}

void MergeTapes::merge()
{
    fillMinValues();
    placeValuesOnDest();
}

void MergeTapes::fillMinValues()
{
    for (const auto& tape : sourceTapes) {
        tape->rewind();
        uint32_t number;
        if (tape->read(number)) {
            minValues.insert({number, tape});
        }
    }
}

void MergeTapes::placeValuesOnDest()
{
    while (!minValues.empty()) {
        placeMinimumValueOnDest();
    }
}

void MergeTapes::placeMinimumValueOnDest()
{
    auto minimumElem = minValues.begin();
    uint32_t anotherValue;
    bool tapeNotEnds = minimumElem->second->read(anotherValue);
    if (tapeNotEnds) {
        minValues.insert({anotherValue, minimumElem->second});
    }
    bool writed = dest->write(minimumElem->first);
    if (!writed) {
        throw std::runtime_error("Can't merge in destination tape");
    }
    minValues.erase(minimumElem);
}

}  // namespace tape
