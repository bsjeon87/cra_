#pragma once
#include "SimilarAlgorithm.h"
#include <memory>

using namespace std;

class SimilarAlgorithmFactory {
public:
	shared_ptr< SimilarAlgorithm> createAlgorithm(string algorithmName);
};