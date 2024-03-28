#include "Merge.h"

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
    uint32_t anotherValue;  //= getAnotherValue(minimumElem->second, tapeEnds);
    bool tapeNotEnds = minimumElem->second->read(anotherValue);
    if (tapeNotEnds) {
        minValues.insert({anotherValue, minimumElem->second});
    }
    // TODO add throw
    dest->write(minimumElem->first);
    minValues.erase(minimumElem);
}
