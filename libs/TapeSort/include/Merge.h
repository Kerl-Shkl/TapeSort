#ifndef TAPESORTALGORITHM_INCLUDE_MERGE_H
#define TAPESORTALGORITHM_INCLUDE_MERGE_H

#include "ITape.h"
#include <map>
#include <memory>
#include <vector>

namespace tape {

class MergeTapes
{
public:
    using TapePtr = std::shared_ptr<tape::ITape>;

    MergeTapes(TapePtr dest, std::vector<TapePtr>& sourceTapes);
    void merge();

private:
    void fillMinValues();
    void placeValuesOnDest();
    void placeMinimumValueOnDest();

    TapePtr dest;
    std::vector<TapePtr>& sourceTapes;
    std::multimap<uint32_t, TapePtr> minValues;
};

}  // namespace tape

#endif  // end of include guard: TAPESORTALGORITHM_INCLUDE_MERGE_H
