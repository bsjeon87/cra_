#include "SimilarAlgorithmFactory.h"
#include "LevenshteinAlgorithm.h"
#include "CorrectorException.h"

shared_ptr<SimilarAlgorithm> SimilarAlgorithmFactory::createAlgorithm(string algorithmName) {
	if (algorithmName == "LevenshteinAlgorithm") {
		return make_shared<LevenshteinAlgorithm>();
	}

	throw CorrectorException("invalid algorithm name");}