#pragma once

#include "SimilarAlgorithm.h"

class LevenshteinAlgorithm : public SimilarAlgorithm {
public:
	virtual bool similar(const std::string& a, const std::string& b);
private:
	int levenshteinAlgorithm(const std::string& a, const std::string& b);
};