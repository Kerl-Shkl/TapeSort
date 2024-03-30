#ifndef INCLUDE_SETTINGSREADER_H
#define INCLUDE_SETTINGSREADER_H

#include <chrono>
#include <string>

struct Settings
{
    using milliseconds = std::chrono::milliseconds;
    uint32_t intermTapeCount = 0;
    uint32_t memoryCapacity = 100;
    milliseconds stepDelay = {};
    milliseconds readDelay = {};
    milliseconds writeDelay = {};
    milliseconds rewindDelay = {};
};

class SettingsReader
{
public:
    SettingsReader(const std::string& filename);
    const Settings& getSettings();

private:
    void parseLine(const std::string& line);
    void setSetting(const std::string& field, uint32_t value);

    std::string filename;
    Settings settings;
};

#endif  // end of include guard: INCLUDE_SETTINGSREADER_H
