

#define UT

#include <fstream>
#include <iostream>
#include "mission1/corrector_fun.h"
#include "mission2/Corrector.h"
#include "mission2/SimilarAlgorithmFactory.h"

using namespace std;

#if defined(UT)
#include <gmock/gmock.h>
#endif


#if defined(MISSION_1)
void test_input_using_mission1() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력

	for (int i = 0; i < 500; i++) {
		string week, word;
		fin >> word >> week;
		string ret = corrector_fun(word, week); // for mission 2
		std::cout << ret << "\n";
	}
}
#elif defined(MISSION_2)
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
#endif

int main(int argc, char* argv[]) {
#if defined(MISSION_1)
	test_input_using_mission1();
	return 0;
#elif defined (MISSION_2)
	string similarAlgorithm = "LevenshteinAlgorithm";
	if (argc == 2) {
		similarAlgorithm = argv[1];
	}
	test_input_using_mission2(similarAlgorithm);
	return 0;
#elif defined(UT)
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
#endif
}