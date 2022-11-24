#pragma once
#include "globals.h"

// The Chromosome
class CSet {
public:
	CSet();
	~CSet();
	CSet*        Combine(CSet* set1, CSet* set2);
	std::string  Mutation();

	unsigned int fitness; // Lower fitness = better
	std::vector<std::string> setInfo; // Set data, series of sentences (genes)
};