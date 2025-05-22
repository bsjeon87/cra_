#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>
#include <memory>
#include "Corrector.h"
#include "LevenshteinAlgorithm.h"

using namespace testing;
using namespace std;

class MockSimilarAlgorithm : public SimilarAlgorithm {
public:
    MOCK_METHOD(bool, similar, (const string& a, const string& b), (override));
};

TEST(SimilarAlgorithm, AlgorithDI) {
    shared_ptr<MockSimilarAlgorithm> mockAlgorithm = make_shared<MockSimilarAlgorithm>();
    Corrector corrector;
    corrector.setSimilarAlgorithm(mockAlgorithm);

    EXPECT_CALL(*mockAlgorithm, similar(StrEq("a"), StrEq("b"))).Times(2)
        .WillOnce(testing::Return(false));  // one for day, one for week

    corrector.processCorrect("a", MON_STR);
    corrector.processCorrect("b", MON_STR);
}

TEST(SimilarAlgorithm, AlgorithDI2) {
    shared_ptr<MockSimilarAlgorithm> mockAlgorithm = make_shared<MockSimilarAlgorithm>();
    Corrector corrector;
    corrector.setSimilarAlgorithm(mockAlgorithm);

    EXPECT_CALL(*mockAlgorithm, similar(StrEq("abc"), StrEq("bb"))).Times(1)
        .WillOnce(testing::Return(true));  // one for day

    corrector.processCorrect("abc", MON_STR);
    EXPECT_EQ("abc", corrector.processCorrect("bb", MON_STR));
}


/*
class MockSsdDriverInterface : public SsdDriverInterface {
public:
    MOCK_METHOD(uint32_t, readSSD, (int LBA), (override));
    MOCK_METHOD(void, writeSSD, (int LBA, uint32_t data), (override));
    MOCK_METHOD(void, eraseSSD, (int LBA, int size), (override));
    MOCK_METHOD(void, flushSSD, (), (override));
};


class ShellExecutorFixture : public testing::Test {
public:
    ShellExecutor shellExecutor;
    string result;
};

TEST_F(ShellExecutorFixture, readWriteTest1) {
    shared_ptr<MockSsdDriverInterface> mockDriver = make_shared<MockSsdDriverInterface>();
    SsdDriverStore::getSsdDriverStore().setSsdDriver(mockDriver);

    EXPECT_CALL(*mockDriver, readSSD(1)).WillOnce(testing::Return(0xAAAABBBB));
    EXPECT_CALL(*mockDriver, writeSSD(1, 0xAAAABBBB)).WillOnce(testing::Return());

    EXPECT_EQ("[Write] Done", shellExecutor.execute("write 01 0xAAAABBBB", false));
    EXPECT_EQ("[Read] LBA 01 : 0xAAAABBBB", shellExecutor.execute("read 01", false));
}
*/