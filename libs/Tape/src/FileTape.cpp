#include "FileTape.h"

namespace tape {

FileTape::FileTape(std::fstream&& tapeFile)
{
    setFstream(std::move(tapeFile));
}

bool FileTape::read(uint32_t& buffer)
{
    if (file.tellg() >= actualEnd) {
        return false;
    }
    file.read(reinterpret_cast<char *>(&buffer), block_size);
    return isOkStateAndClear();
}

bool FileTape::write(uint32_t value)
{
    const auto prePos = file.tellp();
    file.write(reinterpret_cast<char *>(&value), block_size);
    bool writeSuccess = isOkStateAndClear();
    if (writeSuccess && prePos >= actualEnd) {
        actualEnd = file.tellp();
    }
    return writeSuccess;
}

bool FileTape::peek(uint32_t& buffer)
{
    bool readSuccess = read(buffer);
    if (readSuccess) {
        file.seekg(-block_size, std::ios_base::cur);
    }
    return readSuccess;
}

bool FileTape::put(uint32_t value)
{
    bool writeSuccess = write(value);
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
    file.seekg(0, std::ios_base::beg);
    return isOkStateAndClear();
}

bool FileTape::clear()
{
    actualEnd = 0;
    return rewind();
}

void FileTape::setFstream(std::fstream&& stream)
{
    file = std::move(stream);
    file.seekg(0, std::ios_base::end);
    actualEnd = file.tellg();
    file.seekg(0, std::ios_base::beg);
}

bool FileTape::isOkStateAndClear()
{
    if (file.fail()) {
        file.clear();
        return false;
    }
    return true;
}

}  // namespace tape
