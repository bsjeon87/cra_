#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "SimilarAlgorithm.h"

using namespace std;

struct WordDbInfo {
	string name;
	double point;

	bool operator<(const WordDbInfo& other) const {
		return point < other.point;
	}
};

struct CorrectorProcessResult {
	bool isFound;
	string correctedStr;
};

const int WEEKS_NUM = 7;
const int MAX_POINT = 2100000000;
const int MAX_DB_INFO_NUM = 10;

const string MON_STR = "monday";
const string TUE_STR = "tuesday";
const string WED_STR = "wednesday";
const string THU_STR = "thursday";
const string FRI_STR = "friday";
const string SAT_STR = "saturday";
const string SUN_STR = "sunday";

const map<string, int> WEEK_IDX_CONVERT = {
	{MON_STR, 0},
	{TUE_STR, 1},
	{WED_STR, 2},
	{THU_STR, 3},
	{FRI_STR, 4},
	{SAT_STR, 5},
	{SUN_STR, 6},
};

class Corrector {
public:
	string processCorrect(string word, string week);
	void setSimilarAlgorithm(shared_ptr<SimilarAlgorithm> similarAlgorithm);
private:
	vector<WordDbInfo>& getWeekBest(string weekStr);
	void reAlginPointInfo(vector<WordDbInfo>& bestInfo);
	void reAlginPoints();
	double processFullmatchedPoints(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest);
	CorrectorProcessResult processCorrector(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest);
	void processNewWord(string word, double point, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest);
	bool isInvalidInput(string week);

	vector<WordDbInfo> dayOfWeekBest[WEEKS_NUM];
	vector<WordDbInfo> weekdaysBest;
	vector<WordDbInfo> weekendBest;
	int UZ;

	shared_ptr<SimilarAlgorithm> similarAlgorithm;
};