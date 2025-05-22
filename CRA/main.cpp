
#include <fstream>
#include <iostream>
#include "mission1/corrector_fun.h"

using namespace std;

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		WordInputInfo wordInput;
		fin >> wordInput.word >> wordInput.week;
		string ret = corrector_fun(wordInput);
		std::cout << ret << "\n";
	}
}

int main() {
	input();

}