#include "SettingsReader.h"
#include <algorithm>
#include <fstream>

static void ltrim(std::string& s);
static void rtrim(std::string& s);
static void trim(std::string& s);

SettingsReader::SettingsReader(const std::string& filename)
: filename(filename)
{}

const Settings& SettingsReader::getSettings()
{
    std::ifstream in(filename);
    if (!in.is_open()) {
        // TODO throw
        return settings;
    }
    std::string line;
    while (std::getline(in, line)) {
        parseLine(line);
    }
    return settings;
}

void SettingsReader::parseLine(const std::string& line)
{
    const auto separator = line.find(':');
    std::string field = line.substr(0, separator);
    std::string valueStr = line.substr(separator+1, line.size());
    trim(field);
    trim(valueStr);
    uint32_t value = std::stoul(valueStr);
    setSetting(field, value);
}

void SettingsReader::setSetting(const std::string& field, uint32_t value)
{
    if (field == "interm_tape_count") {
        settings.intermTapeCount = value;
    }
    else if (field == "memory_capacity") {
        settings.memoryCapacity = value;
    }
    else if (field == "step_delay") {
        settings.stepDelay = std::chrono::milliseconds(value);
    }
    else if (field == "read_delay") {
        settings.readDelay = std::chrono::milliseconds(value);
    }
    else if (field == "write_delay") {
        settings.writeDelay = std::chrono::milliseconds(value);
    }
    else if (field == "rewind_delay") {
        settings.rewindDelay = std::chrono::milliseconds(value);
    }
}

static void trim(std::string& s)
{
    rtrim(s);
    ltrim(s);
}

static void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

static void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}
