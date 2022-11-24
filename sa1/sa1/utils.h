#pragma once
#include "globals.h"

bool        UTIL_CheckSort( std::vector<CSet> population );
LPCWSTR     UTIL_charTolpsz( const char* c );
std::string UTIL_GetFitness();
void        UTIL_ClearFitness();
std::string UTIL_GetScrape();