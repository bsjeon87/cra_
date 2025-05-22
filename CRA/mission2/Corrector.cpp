
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Corrector.h"

// ������Ÿ�� �Ÿ� ��� �˰��� (���ڿ� ���絵 �˻�)
int Corrector::levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// ���� ȯ��
bool Corrector::similer(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;

	int dist = levenshtein(a, b);
	int max_len = (int)std::max(a.length(), b.length());
	// ���絵 ���� (1.0: ������ ����, 0.0: ���� �ٸ�)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= SIMILARITY_THRESHOLD) return true;
	return false;
}

vector<WordDbInfo>& Corrector::getWeekBest(string weekStr) {
	if (weekStr == "saturday" || weekStr == "sunday") {
		return weekendBest;
	}
	return weekdaysBest;
}

void Corrector::reAlginPointInfo(vector<WordDbInfo>& bestInfo) {
	int num = 1;

	// ���� ���� Ű������� 1��. 
	for (int idx = bestInfo.size() - 1; idx >= 0; idx--) {
		bestInfo[idx].point = num;
		num++;

	}
}

void Corrector::reAlginPoints() {
	UZ = 9;

	for (int i = 0; i < WEEKS_NUM; i++) {
		reAlginPointInfo(dayOfWeekBest[i]);
	}
	reAlginPointInfo(weekendBest);
	reAlginPointInfo(weekdaysBest);
}

double Corrector::processFullmatchedPoints(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
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

CorrectorProcessResult Corrector::processCorrector(string word, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
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
void Corrector::processNewWord(string word, double point, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
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

bool Corrector::isInvalidInput(string week) {
	if (WEEK_IDX_CONVERT.find(week) == WEEK_IDX_CONVERT.end()) {
		return true;
	}
	return false;
}

string Corrector::processCorrect(string word, string week) {

	if (isInvalidInput(week)) {
		return "";
	}

	vector<WordDbInfo>& dayBest = dayOfWeekBest[WEEK_IDX_CONVERT.at(week)];
	vector<WordDbInfo>& weeksBest = getWeekBest(week);

	UZ++;
	if (UZ >= MAX_POINT) {
		reAlginPoints();
	}

	// �Ϻ� HIT Check(���� ��Ͽ� �����ϴ��� Ȯ��/ �����Ǵ� Ű�����̸� ������ ����)
	double perfectHitMaxPoint = processFullmatchedPoints(word, dayBest, weeksBest);
	if (perfectHitMaxPoint > 0) {
		return word;
	}

	//���� 
	CorrectorProcessResult resultOfCorrector = processCorrector(word, dayBest, weeksBest);
	if (resultOfCorrector.isFound) {
		return resultOfCorrector.correctedStr;
	}

	//�Ϻ� HIT / ���� HIT �Ѵ� �ƴѰ��
	processNewWord(word, UZ, dayBest, weeksBest);

	return word;
}