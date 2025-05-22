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
const int SIMILARITY_THRESHOLD = 80;
const map<string, int> WEEK_IDX_CONVERT = {
	{"monday", 0},
	{"tuesday", 1},
	{"wednesday", 2},
	{"thursday", 3},
	{"friday", 4},
	{"saturday", 5},
	{"sunday", 6},
};

class Corrector {
public:
	string processCorrect(string word, string week);
	void setSimilarAlgorithm(shared_ptr<SimilarAlgorithm> similarAlgorithm);
private:
	bool similer(const std::string& a, const std::string& b);
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