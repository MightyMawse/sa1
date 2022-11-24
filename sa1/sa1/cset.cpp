#include "globals.h"

CSet::CSet() {
}

CSet::~CSet() {
}

/*
Combine
combine two sets
*/
CSet* CSet::Combine( CSet* set1, CSet* set2 ) {

	CSet* new_set = new CSet();

	std::vector<std::string> new_setInfo;
	std::vector<std::string> aggregatePiece1, aggregatePiece2;

	// Cut set1 and 2 chromosome into genes
	aggregatePiece1 = set1->setInfo;
	aggregatePiece2 = set2->setInfo;

	// Create new set
	for (int n = 0; n < MAX_SET_SIZE; n++) { // Foreach sentence in a set (50)
		int rnd = G_Random(0, 100);
		// Random probability
		if (rnd <= 45)                  { new_setInfo.push_back(aggregatePiece1[n]); } // Random sentence from set1
		else if (rnd > 45 && rnd <= 90) { new_setInfo.push_back(aggregatePiece2[n]); } // Random sentence from set2
		else                            { new_setInfo.push_back(Mutation());}
	}

	new_set->setInfo = new_setInfo;
	return new_set;
}

/*
Mutation
generate random piece
*/
std::string CSet::Mutation() {
    std::string piece;
    piece = this->setInfo[G_Random(0, SENTENCE_IN_SET)];
	return piece;
}