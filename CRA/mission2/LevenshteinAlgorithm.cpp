#include "LevenshteinAlgorithm.h"
#include <vector>

const int SIMILARITY_THRESHOLD = 80;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int LevenshteinAlgorithm::levenshteinAlgorithm(const std::string& a, const std::string& b) {
	const size_t len_a = (int)a.size();
	const size_t len_b = (int)b.size();

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

bool LevenshteinAlgorithm::similar(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;

	int dist = levenshteinAlgorithm(a, b);
	int max_len = (int)std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= SIMILARITY_THRESHOLD) return true;
	return false;
}