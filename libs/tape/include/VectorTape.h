#ifndef TAPE_INCLUDE_VECTORTAPE_H
#define TAPE_INCLUDE_VECTORTAPE_H

#include "ITape.h"
#include <vector>

class VectorTape : public ITape
{
public:
    virtual ~VectorTape() = default;
    virtual bool read(uint32_t& recipient) override;
    virtual bool write(uint32_t value) override;
    virtual bool stepForward() override;
    virtual bool stepBackward() override;
    virtual bool rewind() override;

    void setVector(std::vector<uint32_t> vec);

private:
    void resizeIfNeeded();

    std::vector<uint32_t> buffer;
    std::vector<uint32_t>::size_type pos = 0;
    size_t actualEnd = 0;
};

#endif  // end of include guard: TAPE_INCLUDE_VECTORTAPE_H
