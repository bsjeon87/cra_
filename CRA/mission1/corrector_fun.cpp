
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "corrector_fun.h"

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

vector<WordDbInfo> dayOfWeekBest[WEEKS_NUM]; //월 ~ 일
vector<WordDbInfo> weekdaysBest;
vector<WordDbInfo> weekendBest; 
int UZ = 9;

const map<string, int> WEEK_IDX_CONVERT = {
	{"monday", 0},
	{"tuesday", 1},
	{"wednesday", 2},
	{"thursday", 3},
	{"friday", 4},
	{"saturday", 5},
	{"sunday", 6},
};

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int levenshtein(const std::string& a, const std::string& b) {
	const int len_a = (int)a.size();
	const int len_b = (int)b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (int i = 0; i <= len_a; ++i) d[i][0] = i;
	for (int j = 0; j <= len_b; ++j) d[0][j] = j;

	for (int i = 1; i <= len_a; ++i) {
		for (int j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// 점수 환산
bool similer(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;

	int dist = levenshtein(a, b);
	int max_len = (int)std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= SIMILARITY_THRESHOLD) return true;
	return false;
}

vector<WordDbInfo>& getWeekBest(string weekStr) {
	if (weekStr == "saturday" || weekStr == "sunday") {
		return weekendBest;
	}
	return weekdaysBest;
}

void reAlginPointInfo(vector<WordDbInfo>& bestInfo) {
	int num = 1;

	// 낮은 순위 키워드부터 1점. 
	for (int idx = (int)bestInfo.size() - 1; idx >= 0; idx--) {
		bestInfo[idx].point = num;
		num++;

	}
}

void reAlginPoints() {
	UZ = 9;

	for (int i = 0; i < WEEKS_NUM; i++) {
		reAlginPointInfo(dayOfWeekBest[i]);
	}
	reAlginPointInfo(weekendBest);
	reAlginPointInfo(weekdaysBest);
}

double processFullmatchedPoints(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
	double perfectHitMaxPoint = 0;

	for (WordDbInfo& node : dayBest) {
		if (node.name == word) {
			node.point += (node.point * 0.1);
			std::sort(dayBest.begin(), dayBest.end());
			perfectHitMaxPoint = node.point;
			break;
		}
	}

	for (WordDbInfo& node : weeksBest) {
		if (node.name == word) {
			node.point += (node.point * 0.1);
			std::sort(weeksBest.begin(), weeksBest.end());
			if (perfectHitMaxPoint < node.point) {
				perfectHitMaxPoint = node.point;
			}
			break;
		}
	}

	if (perfectHitMaxPoint >= MAX_POINT) {
		reAlginPoints();
	}

	return perfectHitMaxPoint;
}

CorrectorProcessResult processCorrector(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
	for (WordDbInfo& node : dayBest) {
		if (similer(node.name, word)) {
			return { true, node.name };
		}
	}

	for (WordDbInfo& node : weeksBest) {
		if (similer(node.name, word)) {
			return { true, node.name };
		}
	}

	return { false, "" };
}
void processNewWord(string word, double point, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
	dayBest.push_back({ word, point });
	std::sort(dayBest.begin(), dayBest.end());
	if (dayBest.size() > MAX_DB_INFO_NUM) {
		dayBest.pop_back();
	}

	weeksBest.push_back({ word, point });
	std::sort(weeksBest.begin(), weeksBest.end());
	if (weeksBest.size() >= MAX_DB_INFO_NUM) {
		weeksBest.pop_back();
	}
}

bool isInvalidInput(string week) {
	if (WEEK_IDX_CONVERT.find(week) == WEEK_IDX_CONVERT.end()) {
		return true;
	}
	return false;
}

string corrector_fun(string word, string week) {

	if (isInvalidInput(week)) {
		return "";
	}

	vector<WordDbInfo>& dayBest = dayOfWeekBest[WEEK_IDX_CONVERT.at(week)];
	vector<WordDbInfo>& weeksBest = getWeekBest(week);
	
	UZ++;
	if (UZ >= MAX_POINT) {
		reAlginPoints();
	}

	// 완벽 HIT Check(관리 목록에 존재하는지 확인/ 관리되는 키워드이면 점수가 증가)
	double perfectHitMaxPoint = processFullmatchedPoints(word, dayBest, weeksBest);
	if (perfectHitMaxPoint > 0) {
		return word;
	}

	//찰떡 
	CorrectorProcessResult resultOfCorrector = processCorrector(word, dayBest, weeksBest);
	if (resultOfCorrector.isFound) {
		return resultOfCorrector.correctedStr;
	}

	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	processNewWord(word, UZ, dayBest, weeksBest);

	return word;
}