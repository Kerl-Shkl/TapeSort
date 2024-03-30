#ifndef TAPE_INCLUDE_VECTORTAPE_H
#define TAPE_INCLUDE_VECTORTAPE_H

#include "ITape.h"
#include <vector>

namespace tape {

class VectorTape : public ITape
{
public:
    ~VectorTape() = default;
    bool read(uint32_t& recipient) override;
    bool write(uint32_t value) override;
    bool peek(uint32_t& recipient) override;
    bool put(uint32_t value) override;
    bool stepForward() override;
    bool stepBackward() override;
    bool rewind() override;
    bool clear() override;

    void setVector(std::vector<uint32_t> vec);
    std::vector<uint32_t>& getVector();
    const std::vector<uint32_t>& getVector() const;

public:
    void resizeIfNeeded();

    std::vector<uint32_t> buffer;
    std::vector<uint32_t>::size_type pos = 0;
    size_t actualEnd = 0;
};

}  // namespace tape

#endif  // end of include guard: TAPE_INCLUDE_VECTORTAPE_H
