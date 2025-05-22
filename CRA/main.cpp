
#include <fstream>
#include <iostream>
#include "mission1/corrector_fun.h"
#include "mission2/Corrector.h"

using namespace std;

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	Corrector corrector;
	for (int i = 0; i < 500; i++) {
		string week, word;
		fin >> word >> week;
		//string ret = corrector_fun(word,week);
		string ret = corrector.processCorrect(word, week);
		std::cout << ret << "\n";
	}
}

int main() {
	input();

}