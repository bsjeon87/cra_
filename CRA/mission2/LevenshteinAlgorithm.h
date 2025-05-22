#pragma once

#include "SimilarAlgorithm.h"

class LevenshteinAlgorithm : public SimilarAlgorithm {
public:
	virtual int getSimilar(const std::string& a, const std::string& b);
};