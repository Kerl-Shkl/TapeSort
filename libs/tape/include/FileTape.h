#ifndef TAPE_INCLUDE_FILETAPE_H
#define TAPE_INCLUDE_FILETAPE_H

#include "ITape.h"
#include <fstream>

class FileTape : public ITape
{
public:
    FileTape(std::fstream&& tapeFile);
    FileTape() = default;
    virtual ~FileTape() override = default;
    FileTape(FileTape&&) = default;
    FileTape& operator=(FileTape&&) = default;

    virtual bool read(uint32_t& buffer) override;
    virtual bool write(uint32_t value) override;
    virtual bool stepForward() override;
    virtual bool stepBackward() override;
    virtual bool rewind() override;

    void setFstream(std::fstream&& stream);

    FileTape(const FileTape&) = delete;
    FileTape& operator=(const FileTape&) = delete;

private:
    bool isOkStateAndClear();

    std::fstream file;
    static constexpr auto block_size = sizeof(uint32_t);
};

#endif  // end of include guard: TAPE_INCLUDE_FILETAPE_H
