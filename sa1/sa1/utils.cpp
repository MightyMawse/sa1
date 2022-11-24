#include "utils.h"

/*
UTIL_CheckSort
check if population is sorted
*/
bool UTIL_CheckSort( std::vector<CSet> population ) {
	for (int i = 0; i < population.size(); i++) {
		if (i > 0) {
			if (population[i].fitness < population[i - 1].fitness) {
				return false;
			}
		}
	}
	return true;
}

/*
UTIL_charTolpsz
parse char to lpcwstr
*/
LPCWSTR UTIL_charTolpsz( const char* c ) {
	const char* w = c;
	WCHAR    str3[50];
	MultiByteToWideChar(0, 0, w, 5, str3, 50);
	LPCWSTR cstr4 = str3;
	return cstr4;
}


/*
UTIL_GetFitness
return fitness from file
*/
std::string UTIL_GetFitness() {
	std::ifstream fitness("F:/" + PLAGARISM_BUFFER);
	std::string read;
	std::getline(fitness, read);
	fitness.close();
	return read;
}

/*
UTIL_GetScrape
return scrapedata from file
*/
std::string UTIL_GetScrape() {
	std::ifstream fitness("F:/" + SCRAPE_BUFFER);
	std::string read;
	std::getline(fitness, read);
	fitness.close();
	return read;
}

/*
UTIL_ClearFitness
self explanatory
*/
void UTIL_ClearFitness() {
	std::ofstream fitness("F:/fitness.txt");
	fitness.clear();
	fitness.close();
}