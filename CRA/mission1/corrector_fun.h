
#pragma once
#include <string>

using namespace std;

struct WordInputInfo {
	string word;
	string week;
};

string corrector_fun(WordInputInfo wordInput);