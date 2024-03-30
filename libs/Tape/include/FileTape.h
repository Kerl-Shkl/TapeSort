#ifndef TAPE_INCLUDE_FILETAPE_H
#define TAPE_INCLUDE_FILETAPE_H

#include "ITape.h"
#include <fstream>

namespace tape {

class FileTape : public ITape
{
public:
    FileTape() = default;
    ~FileTape() override = default;
    FileTape(std::fstream&& tapeFile);

    bool read(uint32_t& buffer) override;
    bool write(uint32_t value) override;
    bool peek(uint32_t& buffer) override;
    bool put(uint32_t value) override;
    bool stepForward() override;
    bool stepBackward() override;
    bool rewind() override;
    bool clear() override;

    void setFstream(std::fstream&& stream);

private:
    bool isOkStateAndClear();

    std::fstream file;
    std::fstream::pos_type actualEnd;
    static constexpr auto block_size = sizeof(uint32_t);
};

}  // namespace tape

#endif  // end of include guard: TAPE_INCLUDE_FILETAPE_H
