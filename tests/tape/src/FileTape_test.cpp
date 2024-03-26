#include "FileTape.h"
#include <filesystem>
#include <gtest/gtest.h>

class FileTapeTest : public testing::Test
{
protected:
    void SetUp() override
    {
        createFile("emptyFile");
        createFile("fullFile");
        fillFullFileWithNumbers();
        setEmptyFile();
    }

    void TearDown() override
    {
        deleteFile("emptyFile");
        deleteFile("fullFile");
    }

    void fillFullFileWithNumbers()
    {
        using std::ios_base;
        std::fstream fullFile("fullFile",
                              ios_base::in | ios_base::out | ios_base::binary);
        for (uint32_t number : numbers) {
            fullFile.write(reinterpret_cast<char *>(&number), sizeof(number));
        }
        fullFile.seekp(0, ios_base::beg);
        fullFile.seekg(0, ios_base::beg);
        full.setFstream(std::move(fullFile));
    }

    void setEmptyFile()
    {
        using std::ios_base;
        std::fstream emptyFile("emptyFile",
                               ios_base::in | ios_base::out | ios_base::binary);
        empty.setFstream(std::move(emptyFile));
    }

    void createFile(const std::string& filename)
    {
        std::ofstream{filename};
    }

    void deleteFile(const std::string& filename)
    {
        std::filesystem::remove(filename);
    }

    std::array<uint32_t, 4> numbers = {12, 31, 15, 65};
    FileTape empty;
    FileTape full;
    FileTape notOpened;
};

TEST_F(FileTapeTest, successReadFromFullFile)
{
    decltype(numbers) readedNumbers;
    for (size_t i = 0; i < numbers.size(); ++i) {
        uint32_t number;
        if (full.read(number)) {
            readedNumbers[i] = number;
            full.stepForward();
        }
        else {
            break;
        }
    }

    EXPECT_EQ(numbers, readedNumbers);
}

TEST_F(FileTapeTest, errorReadFromEmptyFile)
{
    uint32_t number;
    EXPECT_FALSE(empty.read(number));
}

TEST_F(FileTapeTest, errorOnStepBeforeBegin)
{
    full.rewind();
    EXPECT_FALSE(full.stepBackward());
}

TEST_F(FileTapeTest, writeRewindRead)
{
    for (size_t i = 0; i < numbers.size(); ++i) {
        empty.write(numbers[i]);
    }
    empty.rewind();
    decltype(numbers) readedNumbers;
    for (size_t i = 0; i < numbers.size(); ++i) {
        uint32_t number;
        if (full.read(number)) {
            readedNumbers[i] = number;
            full.stepForward();
        }
        else {
            break;
        }
    }

    EXPECT_EQ(numbers, readedNumbers);
}

TEST_F(FileTapeTest, allErrorWithNotOpened)
{
    uint32_t number = 0;
    EXPECT_FALSE(notOpened.write(number));
    EXPECT_FALSE(notOpened.read(number));
    EXPECT_FALSE(notOpened.stepForward());
    EXPECT_FALSE(notOpened.stepBackward());
    EXPECT_FALSE(notOpened.rewind());
}
