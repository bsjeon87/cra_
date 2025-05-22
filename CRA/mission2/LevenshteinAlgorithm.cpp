#include "LevenshteinAlgorithm.h"
#include <vector>

const int SIMILARITY_THRESHOLD = 80;

// ������Ÿ�� �Ÿ� ��� �˰��� (���ڿ� ���絵 �˻�)
int LevenshteinAlgorithm::levenshteinAlgorithm(const std::string& inputStr1, const std::string& inputStr2) {
	const size_t len_inputStr1 = (int)inputStr1.size();
	const size_t len_inputStr2 = (int)inputStr2.size();

	std::vector<std::vector<int>> distVec(len_inputStr1 + 1, std::vector<int>(len_inputStr2 + 1));

	for (int i = 0; i <= len_inputStr1; ++i) distVec[i][0] = i;
	for (int j = 0; j <= len_inputStr2; ++j) distVec[0][j] = j;

	for (int i = 1; i <= len_inputStr1; ++i) {
		for (int j = 1; j <= len_inputStr2; ++j) {
			if (inputStr1[i - 1] == inputStr2[j - 1])
				distVec[i][j] = distVec[i - 1][j - 1];
			else
				distVec[i][j] = 1 + std::min({ distVec[i - 1][j], distVec[i][j - 1], distVec[i - 1][j - 1] });
		}
	}
	return distVec[len_inputStr1][len_inputStr2];
}

bool LevenshteinAlgorithm::similar(const std::string& inputStr1, const std::string& inputStr2) {
	if (inputStr1.empty() && inputStr2.empty()) return true;
	if (inputStr1.empty() || inputStr2.empty()) return false;

	int dist = levenshteinAlgorithm(inputStr1, inputStr2);
	int max_len = (int)std::max(inputStr1.length(), inputStr2.length());
	// ���絵 ���� (1.0: ������ ����, 0.0: ���� �ٸ�)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= SIMILARITY_THRESHOLD) return true;
	return false;
}