#ifndef INCLUDE_APPLICATION_H
#define INCLUDE_APPLICATION_H

#include "SettingsReader.h"
#include "TapeTimeModel.h"
#include <memory>
#include <string>
#include <vector>

class Application
{
public:
    Application(int argc, char *argv[]);
    void run();

public:
    using TapePtr = std::shared_ptr<tape::TapeTimeModel>;

    void doSort();
    bool parseArgs(int argc, char *argv[]);
    void printUsage(const std::string& exec_name) const;
    void createTapes();
    void createIntermTapes();
    void setTimeSettings(TapePtr& tape) const;
    void checkSettings() const;

    Settings settings;
    std::string settingsFilename = "settings.txt";
    std::string sourceFilename;
    std::string destinationFilename;

    TapePtr source;
    TapePtr destination;
    std::vector<TapePtr> intermTapes;
};

#endif  // end of include guard: INCLUDE_APPLICATION_H
