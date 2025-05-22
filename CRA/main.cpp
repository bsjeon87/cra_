
#include <fstream>
#include <iostream>
#include "mission1/corrector_fun.h"
#include "mission2/Corrector.h"
#include "mission2/SimilarAlgorithmFactory.h"

using namespace std;

void input(string str) {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력

	SimilarAlgorithmFactory similarAlgorithmFactory;

	Corrector corrector;
	corrector.setSimilarAlgorithm(similarAlgorithmFactory.createAlgorithm(str));

	for (int i = 0; i < 500; i++) {
		string week, word;
		fin >> word >> week;
		//string ret = corrector_fun(word,week); // for mission 1
		string ret = corrector.processCorrect(word, week); // for mission 2
		std::cout << ret << "\n";
	}
}

int main(int argc, char* argv[]) {
	string similarAlgorithm = "LevenshteinAlgorithm";
	if (argc == 2) {
		similarAlgorithm = argv[1];
	}
	input(similarAlgorithm);

	return 0;
}