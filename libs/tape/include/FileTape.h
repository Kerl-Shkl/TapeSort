#ifndef TAPE_INCLUDE_FILETAPE_H
#define TAPE_INCLUDE_FILETAPE_H

#include "ITape.h"
#include <fstream>

class FileTape : public ITape
{
public:
    FileTape() = default;
    virtual ~FileTape() override = default;
    FileTape(std::fstream&& tapeFile);

    virtual bool read(uint32_t& buffer) override;
    virtual bool write(uint32_t value) override;
    virtual bool peek(uint32_t& buffer) override;
    virtual bool put(uint32_t value) override;
    virtual bool stepForward() override;
    virtual bool stepBackward() override;
    virtual bool rewind() override;
    virtual bool clear() override;

    void setFstream(std::fstream&& stream);

private:
    bool isOkStateAndClear();

    std::fstream file;
    std::fstream::pos_type actualEnd;
    static constexpr auto block_size = sizeof(uint32_t);
};

#endif  // end of include guard: TAPE_INCLUDE_FILETAPE_H
