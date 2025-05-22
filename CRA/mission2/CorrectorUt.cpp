#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>
#include <memory>
#include "Corrector.h"
#include "SimilarAlgorithmFactory.h"
#include "LevenshteinAlgorithm.h"
#include "CorrectorException.h"

using namespace testing;
using namespace std;

class MockSimilarAlgorithm : public SimilarAlgorithm {
public:
    MOCK_METHOD(bool, similar, (const std::string& inputStr1, const std::string& inputStr2), (override));
};

class SimilarSetAlgorithm : public SimilarAlgorithm{
public:
    SimilarSetAlgorithm(bool retVal) {
        this->retVal = retVal;
    }
    virtual bool similar(const std::string& inputStr1, const std::string& inputStr2) {
        return retVal;
    }
private:
    bool retVal;
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

TEST(SimilarAlgorithmFactory, wrongAlgorithm) {
    SimilarAlgorithmFactory similarAlgorithmFactory;

    EXPECT_THROW(similarAlgorithmFactory.createAlgorithm("dd"), CorrectorException);
}

TEST(Collator, wrongWeek) {
    Corrector corrector;
    SimilarAlgorithmFactory similarAlgorithmFactory;

    corrector.setSimilarAlgorithm(similarAlgorithmFactory.createAlgorithm("LevenshteinAlgorithm"));
    EXPECT_THROW(corrector.processCorrect("abc", "k"), CorrectorException);
}

TEST(Collator, weekends) {
    shared_ptr<MockSimilarAlgorithm> mockAlgorithm = make_shared<MockSimilarAlgorithm>();
    Corrector corrector;
    corrector.setSimilarAlgorithm(mockAlgorithm);

    EXPECT_CALL(*mockAlgorithm, similar(StrEq("abc"), StrEq("bb"))).Times(1)
        .WillOnce(testing::Return(true));  // one for weekends

    corrector.processCorrect("abc", SAT_STR);
    EXPECT_EQ("abc", corrector.processCorrect("bb", SUN_STR));
}

TEST(Collator, realignUT) {
    shared_ptr<SimilarSetAlgorithm> alwaysTrue = make_shared<SimilarSetAlgorithm>(true);
    Corrector corrector;
    corrector.setSimilarAlgorithm(alwaysTrue);
    corrector.setStartUZ(MAX_POINT - 10);

    corrector.processCorrect("abc", SAT_STR);

    for (int i = 0; i < 20; i++) {

        EXPECT_NO_THROW({
            corrector.processCorrect("abcd", SAT_STR);
         });
    }
}

TEST(Collator, realignUT2) {
    shared_ptr<SimilarSetAlgorithm> alwaysTrue = make_shared<SimilarSetAlgorithm>(true);
    Corrector corrector;
    corrector.setSimilarAlgorithm(alwaysTrue);
    corrector.setStartUZ(MAX_POINT - 10);

    corrector.processCorrect("abc", SAT_STR);

    for (int i = 0; i < 20; i++) {
        EXPECT_NO_THROW({
            EXPECT_EQ("abc", corrector.processCorrect("abc", SAT_STR));
            });
    }
}

TEST(Collator, realignUT3) {
    shared_ptr<SimilarSetAlgorithm> alwaysTrue = make_shared<SimilarSetAlgorithm>(true);
    Corrector corrector;
    corrector.setSimilarAlgorithm(alwaysTrue);
    corrector.setStartUZ(MAX_POINT - 10);

    corrector.processCorrect("abc", SAT_STR);

    for (int i = 0; i < 20; i++) {
        EXPECT_NO_THROW({
            EXPECT_EQ("abc", corrector.processCorrect("abc", SUN_STR));
            });
    }
}

TEST(Collator, popTest) {
    shared_ptr<SimilarSetAlgorithm> alwaysTrue = make_shared<SimilarSetAlgorithm>(false);
    Corrector corrector;
    corrector.setSimilarAlgorithm(alwaysTrue);

    string input = "a";

    for (int i = 0; i < 11; i++) {

        EXPECT_NO_THROW({
            corrector.processCorrect(input, SUN_STR);
            });
        input += "a";
    }
}

TEST(Collator, noAlgorithm) {
    Corrector corrector;

    EXPECT_THROW(corrector.processCorrect("aa", MON_STR), CorrectorException);
}

TEST(LevenshteinAlgorithm, ut1) {
    LevenshteinAlgorithm algorithm;

    EXPECT_EQ(true, algorithm.similar("aa", "aa"));
    EXPECT_EQ(false, algorithm.similar("aac", "aa"));
    EXPECT_EQ(false, algorithm.similar("aac", "aab"));
    EXPECT_EQ(false, algorithm.similar("", "aab"));
    EXPECT_EQ(true, algorithm.similar("", ""));
}