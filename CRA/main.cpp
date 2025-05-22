
#include <fstream>
#include <iostream>
#include "mission1/corrector_fun.h"
#include "mission2/Corrector.h"
#include "mission2/SimilarAlgorithmFactory.h"

using namespace std;

void test_input_using_mission1() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력

	for (int i = 0; i < 500; i++) {
		string week, word;
		fin >> word >> week;
		string ret = corrector_fun(word, week); // for mission 2
		std::cout << ret << "\n";
	}
}

void test_input_using_mission2(string str) {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력

	SimilarAlgorithmFactory similarAlgorithmFactory;

	Corrector corrector;
	corrector.setSimilarAlgorithm(similarAlgorithmFactory.createAlgorithm(str));

	for (int i = 0; i < 500; i++) {
		string week, word;
		fin >> word >> week;
		string ret = corrector.processCorrect(word, week); // for mission 2
		std::cout << ret << "\n";
	}
}
#define MISSION_2
int main(int argc, char* argv[]) {
#if defined(MISSION_1)
	test_input_using_mission1();
#elif defined (MISSION_2)
	string similarAlgorithm = "LevenshteinAlgorithm";
	if (argc == 2) {
		similarAlgorithm = argv[1];
	}
	test_input_using_mission2(similarAlgorithm);
#endif
	return 0;
}