#include "FileTape.h"
#include "TapeTimeModel.h"
#include <iostream>

int main()
{
    using namespace std::chrono_literals;
    std::ofstream file("test_file");
    file.close();
    std::fstream stream("test_file", std::ios_base::in | std::ios_base::out |
                                         std::ios_base::binary);
    auto tape = std::make_unique<tape::FileTape>(std::move(stream));
    tape::TapeTimeModel timeTape(std::move(tape));
    timeTape.setReadDelay(1000ms);
    timeTape.setWriteDelay(500ms);
    timeTape.setStepDelay(2000ms);
    timeTape.setRewindDelay(100ms);

    uint32_t number = 42;
    std::cout << "write" << std::endl;
    timeTape.write(number);
    std::cout << "read" << std::endl;
    timeTape.read(number);
    std::cout << "step" << std::endl;
    timeTape.stepForward();
    std::cout << "rewind" << std::endl;
    timeTape.rewind();
    std::cout << "clear" << std::endl;
    timeTape.clear();
}
