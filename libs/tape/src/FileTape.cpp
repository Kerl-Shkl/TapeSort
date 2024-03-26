#include "FileTape.h"

FileTape::FileTape(std::fstream&& tapeFile)
: file(std::move(tapeFile))
{}

bool FileTape::read(uint32_t& buffer)
{
    file.read(reinterpret_cast<char *>(&buffer), block_size);
    bool readSuccess = isOkStateAndClear();
    if (readSuccess) {
        file.seekg(-block_size, std::ios_base::cur);
    }
    return readSuccess;
}

bool FileTape::write(uint32_t value)
{
    file.write(reinterpret_cast<char *>(&value), block_size);
    bool writeSuccess = isOkStateAndClear();
    if (writeSuccess) {
        file.seekp(-block_size, std::ios_base::cur);
    }
    return writeSuccess;
}

bool FileTape::stepForward()
{
    file.seekg(block_size, std::ios_base::cur);
    return isOkStateAndClear();
}

bool FileTape::stepBackward()
{
    file.seekg(-block_size, std::ios_base::cur);
    return isOkStateAndClear();
}

bool FileTape::rewind()
{
    file.seekp(0, std::ios_base::beg);
    return isOkStateAndClear();
}

void FileTape::setFstream(std::fstream&& stream)
{
    file = std::move(stream);
}

bool FileTape::isOkStateAndClear()
{
    if (file.fail()) {
        file.clear();
        return false;
    }
    return true;
}
