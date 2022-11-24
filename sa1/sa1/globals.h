#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
#include "cset.h"
#include "utils.h"

// Module path index
#define MOD_PLAGARISM 0
#define MOD_SCRAPE 1

#define MAX_SET_SIZE 50 // 50 sentences to an essay

// Widget types
#define TYPE_BTN 1
#define TYPE_TXTBX 2
#define TYPE_STC 3
// Message handler types
#define MSG_INFO 10
#define MSG_WARN 12
#define MSG_ERROR 13

#define SENTENCE_IN_SET 20
#define CSET_INSTANCES 10

// Global variables/handles
extern HWND* parentWin;
const std::string MODULE_PATH[2] = { "mod/mod_plagarism.py", "mod/mod_scrape.py" };

// Names of files used to transfer data
const std::string SCRAPE_BUFFER    = "scrape.txt";
const std::string KEYWORD_BUFFER   = "keyword.txt";
const std::string PLAGARISM_BUFFER = "fitness.txt";

// Screen pos class
class CVec2D {
public:
	CVec2D( unsigned int _x, unsigned int _y );
	~CVec2D();
	unsigned int x;
	unsigned int y;
};

HWND                     G_CreateWidget( unsigned int type, CVec2D pos, CVec2D size, LPCWSTR txt );
void                     G_MessageHandler( unsigned int type, LPCWSTR msg );
void                     G_WinInit();
std::vector<std::string> G_GetPiece( std::string set );
unsigned int             G_Random( unsigned int min, unsigned int max );
CSet*                    G_GenerateFittestSet( std::string keyword );
std::string              G_CompileSet( std::vector<std::string> set );
float                    G_PlagarismCheck( std::string compiledSet );
std::vector<CSet>        G_SortFitness( std::vector<CSet> population );
std::vector<CSet>        G_GetPopulation( std::string keyword );
std::string              G_ReadFile( std::string path );
bool                     G_WriteFile( std::string path, std::string content );