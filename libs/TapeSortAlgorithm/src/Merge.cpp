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
            tape->stepForward();
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
    bool tapeEnds;
    uint32_t anotherValue = getAnotherValue(minimumElem->second, tapeEnds);
    if (!tapeEnds) {
        minValues.insert({anotherValue, minimumElem->second});
    }
    // TODO add throw
    dest->write(minimumElem->first);
    dest->stepForward();
    minValues.erase(minimumElem);
}

uint32_t MergeTapes::getAnotherValue(TapePtr tape, bool& end)
{
    uint32_t result;
    if (tape->read(result)) {
        tape->stepForward();
        end = false;
    }
    else {
        end = true;
    }
    return result;
}
