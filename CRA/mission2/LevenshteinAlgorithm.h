#pragma once

#include "SimilarAlgorithm.h"

class LevenshteinAlgorithm : public SimilarAlgorithm {
public:
	virtual bool similar(const std::string& inputStr1, const std::string& inputStr2);
private:
	int levenshteinAlgorithm(const std::string& inputStr1, const std::string& inputStr2);
};