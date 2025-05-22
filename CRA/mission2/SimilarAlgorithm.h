#pragma once

#include <string>
using namespace std;

class SimilarAlgorithm {
public:
	virtual bool similar(const std::string& inputStr1, const std::string& inputStr2) = 0;
};