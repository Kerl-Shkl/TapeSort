#include "Application.h"
#include "FileTape.h"
#include "TapeSort.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Application::Application(int argc, char *argv[])
{
    if (!parseArgs(argc, argv)) {
        printUsage(argv[0]);
        exit(1);
    }
    SettingsReader sr(settingsFilename);
    settings = sr.getSettings();
    checkSettings();
}

void Application::run()
{
    createTapes();
    TapeSort sorter(source, destination, settings.memoryCapacity);
    sorter.sort();
}

bool Application::parseArgs(int argc, char *argv[])
{
    if (argc != 3 && argc != 5) {
        return false;
    }
    sourceFilename = argv[1];
    destinationFilename = argv[2];
    if (argc == 5) {
        std::string settingsFlag = argv[3];
        if (settingsFlag != "--settings") {
            return false;
        }
        settingsFilename = argv[4];
    }
    return true;
}

void Application::printUsage(const std::string& exec_name) const
{
    std::cout
        << "Usage: " << exec_name
        << " source destination [--settings filename (default = settings.txt)]"
        << std::endl;
}

void Application::createTapes()
{
    using std::ios_base;
    std::ofstream{destinationFilename};  // create dest file
    std::fstream srcStream(sourceFilename,
                           ios_base::in | ios_base::out | ios_base::binary);
    if (!srcStream.is_open()) {
        std::cerr << "Can't open source file: " << sourceFilename << std::endl;
        exit(3);
    }
    std::fstream dstStream(destinationFilename,
                           ios_base::in | ios_base::out | ios_base::binary);
    if (!dstStream.is_open()) {
        std::cerr << "Can't open destination file: " << destinationFilename
                  << std::endl;
        exit(3);
    }

    auto srcFileTape = std::make_unique<tape::FileTape>(std::move(srcStream));
    source = std::make_shared<tape::TapeTimeModel>(std::move(srcFileTape));
    setTimeSettings(source);

    auto dstFileTape = std::make_unique<tape::FileTape>(std::move(dstStream));
    destination = std::make_shared<tape::TapeTimeModel>(std::move(srcFileTape));
    setTimeSettings(destination);

    createIntermTapes();
}

void Application::createIntermTapes()
{
    namespace fs = std::filesystem;
    using std::ios_base;
    if (!fs::exists("tmp") || !fs::is_directory("tmp")) {
        bool created = fs::create_directory("temp");
        if (!created) {
            std::cerr << "Can't create directory \"temp\"" << std::endl;
            exit(4);
        }
    }
    const std::string commonFilename = "tmp/interm";
    for (uint32_t i = 0; i < settings.intermTapeCount; ++i) {
        std::string filename = commonFilename + std::to_string(i + 1);
        std::ofstream{filename, ios_base::trunc};
        std::fstream stream(filename,
                            ios_base::in | ios_base::out | ios_base::binary);
        auto fileTape = std::make_unique<tape::FileTape>(std::move(stream));
        auto timeTape =
            std::make_shared<tape::TapeTimeModel>(std::move(fileTape));
        setTimeSettings(timeTape);
        intermTapes.emplace_back(std::move(timeTape));
    }
}

void Application::setTimeSettings(TapePtr& tape) const
{
    tape->setReadDelay(settings.readDelay);
    tape->setWriteDelay(settings.writeDelay);
    tape->setStepDelay(settings.stepDelay);
    tape->setRewindDelay(settings.rewindDelay);
}

void Application::checkSettings() const
{
    if (settings.intermTapeCount < 1) {
        std::cout << "The number of intermediate tapes must be at least one"
                  << std::endl;
        exit(2);
    }
    if (settings.memoryCapacity < 1) {
        std::cout << "The size of memory capacity must be at least one (one "
                     "block = sizeof(uint32_t))"
                  << std::endl;
        exit(2);
    }
}
