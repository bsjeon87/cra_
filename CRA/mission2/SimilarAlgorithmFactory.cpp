#include "SimilarAlgorithmFactory.h"
#include "LevenshteinAlgorithm.h"

shared_ptr<SimilarAlgorithm> SimilarAlgorithmFactory::createAlgorithm(string algorithmName) {
	if (algorithmName == "LevenshteinAlgorithm") {
		return make_shared<LevenshteinAlgorithm>();
	}

	return nullptr;
}