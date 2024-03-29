#ifndef TAPE_INCLUDE_TAPETIMEMODEL_H
#define TAPE_INCLUDE_TAPETIMEMODEL_H

#include "ITape.h"
#include <chrono>
#include <memory>

namespace tape {

class TapeTimeModel : public tape::ITape
{
public:
    using milliseconds = std::chrono::milliseconds;

    TapeTimeModel(std::unique_ptr<ITape> origin);
    ~TapeTimeModel() override = default;
    bool read(uint32_t& buffer) override;
    bool write(uint32_t value) override;
    bool peek(uint32_t& buffer) override;
    bool put(uint32_t value) override;
    bool stepForward() override;
    bool stepBackward() override;
    bool rewind() override;
    bool clear() override;

    void setStepDelay(const milliseconds& delay);
    void setReadDelay(const milliseconds& delay);
    void setWriteDelay(const milliseconds& delay);
    void setRewindDelay(const milliseconds& delay);

private:
    milliseconds stepDelay = {};
    milliseconds readDelay = {};
    milliseconds writeDelay = {};
    milliseconds rewindDelay = {};
    std::unique_ptr<tape::ITape> tape;
};

}  // namespace tape

#endif  // end of include guard: TAPE_INCLUDE_TAPETIMEMODEL_H
