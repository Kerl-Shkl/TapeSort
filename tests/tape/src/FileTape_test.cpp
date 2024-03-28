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
        full.setFstream(std::move(fullFile));
        full.rewind();
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
    uint32_t number;
    for (size_t i = 0; i < numbers.size(); ++i) {
        bool readed = full.read(number);
        ASSERT_TRUE(readed);
        ASSERT_EQ(number, numbers[i]);
    }
    EXPECT_FALSE(full.peek(number));
}

TEST_F(FileTapeTest, errorReadFromEmptyFile)
{
    uint32_t number;
    EXPECT_FALSE(empty.peek(number));
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

    uint32_t number;
    for (size_t i = 0; i < numbers.size(); ++i) {
        bool readed = empty.read(number);
        ASSERT_TRUE(readed);
        ASSERT_EQ(number, numbers[i]);
    }
    EXPECT_FALSE(empty.peek(number));
}

TEST_F(FileTapeTest, clearFullFile)
{
    uint32_t number;
    EXPECT_TRUE(full.peek(number));
    full.clear();
    EXPECT_FALSE(full.peek(number));
}

TEST_F(FileTapeTest, allErrorWithNotOpened)
{
    uint32_t number = 0;
    EXPECT_FALSE(notOpened.put(number));
    EXPECT_FALSE(notOpened.peek(number));
    EXPECT_FALSE(notOpened.stepForward());
    EXPECT_FALSE(notOpened.stepBackward());
    EXPECT_FALSE(notOpened.rewind());
}
