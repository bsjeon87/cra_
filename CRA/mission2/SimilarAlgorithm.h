#pragma once

#include <string>
using namespace std;

class SimilarAlgorithm {
public:
	virtual int getSimilar(const std::string& a, const std::string& b) = 0;
};