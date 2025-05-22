
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Corrector.h"

void Corrector::setSimilarAlgorithm(shared_ptr<SimilarAlgorithm> similarAlgorithm) {
	this->similarAlgorithm = similarAlgorithm;
}

vector<WordDbInfo>& Corrector::getWeekBest(string weekStr) {
	if (weekStr == "saturday" || weekStr == "sunday") {
		return weekendBest;
	}
	return weekdaysBest;
}

void Corrector::reAlginPointInfo(vector<WordDbInfo>& bestInfo) {
	int num = 1;

	// 낮은 순위 키워드부터 1점. 
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
		if (similarAlgorithm->similar(node.name, word)) {
			return { true, node.name };
		}
	}

	for (WordDbInfo& node : weeksBest) {
		if (similarAlgorithm->similar(node.name, word)) {
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

	if (similarAlgorithm == nullptr) {
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