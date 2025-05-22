#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct WordInputInfo {
	string word;
	string week;
};

struct WordDbInfo {
	string name;
	double point;

	bool operator<(const WordDbInfo& other) const {
		return point < other.point;
	}
};

const int WEEKS_NUM = 7;
const int MAX_POINT = 2100000000;

vector<WordDbInfo> dayOfWeekBest[WEEKS_NUM]; //�� ~ ��
vector<WordDbInfo> weekdaysBest;
vector<WordDbInfo> weekendBest; 
int UZ = 9;

map<string, int> gIndexConvert = {
	{"monday", 0},
	{"tuesday", 1},
	{"wednesday", 2},
	{"thursday", 3},
	{"friday", 4},
	{"saturday", 5},
	{"sunday", 6},
};

// ������Ÿ�� �Ÿ� ��� �˰��� (���ڿ� ���絵 �˻�)
int levenshtein(const std::string& a, const std::string& b) {
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
bool similer(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;

	int dist = levenshtein(a, b);
	int max_len = (int)std::max(a.length(), b.length());
	// ���絵 ���� (1.0: ������ ����, 0.0: ���� �ٸ�)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
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

	// ���� ���� Ű������� 1��. 
	for (int idx = bestInfo.size() - 1; idx >= 0; idx--) {
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

void processNewWord(string word, double point, vector<WordDbInfo>& dayBest, vector<WordDbInfo>& weeksBest) {
	if (dayBest.size() < 10) {
		dayBest.push_back({ word, point });
	}

	if (weeksBest.size() < 10) {
		weeksBest.push_back({ word, point });
	}

	if (dayBest.size() == 10) {
		if (dayBest.back().point < point) {
			dayBest.pop_back();
			dayBest.push_back({ word, point });
		}
	}

	if (weeksBest.size() == 10) {
		if (weeksBest.back().point < point) {
			weeksBest.pop_back();
			weeksBest.push_back({ word, point });
		}
	}

	std::sort(dayBest.begin(), dayBest.end());
	std::sort(weeksBest.begin(), weeksBest.end());
}

string corrector(WordInputInfo wordInput) {

	//���� ��Ͽ� �����ϴ��� Ȯ��
	//�����Ǵ� Ű�����̸� ������ ����
	vector<WordDbInfo>& dayBest = dayOfWeekBest[gIndexConvert[wordInput.week]];
	vector<WordDbInfo>& weeksBest = getWeekBest(wordInput.week);
	
	double point = UZ + 1;
	double perfectHitMaxPoint = 0;

	UZ++;

	for (WordDbInfo& node : dayBest) {
		if (node.name == wordInput.word) {
			node.point += (node.point * 0.1);
			std::sort(dayBest.begin(), dayBest.end());
			perfectHitMaxPoint = node.point;
			break;
		}
	}

	for (WordDbInfo& node : weeksBest) {
		if (node.name == wordInput.word) {
			node.point += (node.point * 0.1);
			std::sort(weeksBest.begin(), weeksBest.end());
			if (perfectHitMaxPoint < node.point) {
				perfectHitMaxPoint = node.point;
			}
			break;
		}
	}

	//������ �۾�
	if (UZ >= MAX_POINT || perfectHitMaxPoint >= MAX_POINT) {
		reAlginPoints();
	}

	if (perfectHitMaxPoint > 0) {
		return wordInput.word;
	}

	//���� HIT
	for (WordDbInfo& node : dayBest) {
		if (similer(node.name, wordInput.word)) {
			return node.name;
		}
	}

	for (WordDbInfo& node : weeksBest) {
		if (similer(node.name, wordInput.word)) {
			return node.name;
		}
	}

	//�Ϻ� HIT / ���� HIT �Ѵ� �ƴѰ��
	processNewWord(wordInput.word, point, dayBest, weeksBest);

	return wordInput.word;
}

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500�� ������ �Է�
	for (int i = 0; i < 500; i++) {
		WordInputInfo wordInput;
		fin >> wordInput.word >> wordInput.week;
		string ret = corrector(wordInput);
		std::cout << ret << "\n";
	}
}

int main() {
	input();

}