#include "globals.h"

HWND* parentWin; // Parent window

CVec2D::CVec2D( unsigned int _x, unsigned int _y ) {
	x = _x;
	y = _y;
}

CVec2D::~CVec2D() {
}


/*
G_CreateWidget
create widgets on screen
*/
HWND G_CreateWidget( unsigned int type, CVec2D pos, CVec2D size, LPCWSTR name ) {

	LPCWSTR b_name = ((name == NULL) ? L"new" : name);

	switch (type)
	{
	    case TYPE_BTN: {
			// Button
			HWND btn = CreateWindowW(L"BUTTON",
				b_name, 
				WS_CHILD | WS_VISIBLE, 
				pos.x, pos.y, size.x, size.y, 
				*parentWin, 
				NULL, NULL, NULL);

			return btn;
		}
		case TYPE_TXTBX: {
			// Textbox
			HWND txtbx = CreateWindowW(L"EDIT", 
				b_name, 
				WS_CHILD | WS_VISIBLE | WS_DLGFRAME, 
				pos.x, pos.y, size.x, size.y, 
				*parentWin, 
				NULL, NULL, NULL);

			return txtbx;
		}
		case TYPE_STC: {
			// Static text
			HWND stc = CreateWindowW(L"STATIC", 
				b_name, 
				WS_CHILD | WS_VISIBLE | WS_BORDER, 
				pos.x, pos.y, size.x, size.y, 
				*parentWin, 
				NULL, NULL, NULL);

			return stc;
		}
	}
	return NULL;
}

/*
G_MessageHandler
handles all output messages to user
*/
void G_MessageHandler( unsigned int type, LPCWSTR msg ) {
	switch (type)
	{
	    case MSG_INFO: {
			MessageBoxW(*parentWin, msg, L"sa1 Info", MB_OK | MB_ICONINFORMATION);
			MessageBeep(MB_ICONINFORMATION);
			break;
	    }
		case MSG_WARN: {
			MessageBoxW(*parentWin, msg, L"sa1 Warning", MB_OK | MB_ICONWARNING);
			MessageBeep(MB_ICONWARNING);
			break;
		}
		case MSG_ERROR: {
			MessageBoxW(*parentWin, msg, L"sa1 Error", MB_OK | MB_ICONERROR);
			MessageBeep(MB_ICONERROR);
			break;
		}
	}
}

/*
G_GetPiece
cut string into words(pieces)
*/
std::vector<std::string> G_GetPiece( std::string set ) {

	std::vector<std::string> pieceList;
	std::string currentPiece;

	for (int n = 0; n < set.length(); n++) {
		if (set[n] != ' ') {
			currentPiece += set[n]; // Build piece
		}
		else {
			pieceList.push_back(currentPiece); // Add piece
			currentPiece = "";
		}
	}

	return pieceList;
}

/*
G_Random
generate random int between ranges
*/
unsigned int G_Random( unsigned int min, unsigned int max ) {
	srand(time(NULL));
	return rand() % max + min;
}

/*
G_GenerateFittestSet
generate fittest set possible
*/
CSet* G_GenerateFittestSet( std::string keyword ) {
	/*
	-If population is empty produce random set(chromosome/population) from reading random sentences
	-Compile set into singular string and plagarism check
	-Sort in order of fitness
	-Add fittest 10% to new generation
	-Combine random 50% of generation x40
	-Add offspring to new generation
	-Set population = new generation
	*/

	// Write keyword to file
	std::ofstream temp("F:/" + KEYWORD_BUFFER);
	temp.clear();
	temp << keyword;
	temp.close();

	std::vector<CSet> population; // Current population
	bool eliteBuild = false;

	if (population.size() == 0) {
		for (int i = 0; i < MAX_SET_SIZE; i++) {
			// Web scrape and add random CSet's
			population = G_GetPopulation(keyword);
		}
	}

	while (!eliteBuild) {

		std::vector<CSet> new_generation;

		// Plagarism Check
		for (int n = 0; n < MAX_SET_SIZE; n++) {

			unsigned int fitness;
			std::string compiled = G_CompileSet(population[n].setInfo); // Compile

			fitness = G_PlagarismCheck(compiled); // Get fitness score
			population[n].fitness = fitness; // Set fitness
		}

		// Sort in order of fitness (lo->hi) - fittest at start
		population = G_SortFitness(population);

		if (population[0].fitness == 0) { // Check if eliteBuild exists
			eliteBuild = true;
			break;
		}

		for (int i = 0; i < (MAX_SET_SIZE * 0.1f); i++) { // Add top 10% to new generation
			new_generation.push_back(population[i]);
		}

		// Combine sets x40
		for (int j = 0; j < (MAX_SET_SIZE - 10); j++) {
			unsigned int rnd = G_Random(0, 50);

			CSet offspring = *population[rnd].Combine(&population[rnd],
				&population[G_Random(0, 50)]);

			new_generation.push_back(offspring);
		}

		population = new_generation;
	}

	return &population[0];
}

/*
G_CompileSet
Compile set into singular string
*/
std::string G_CompileSet( std::vector<std::string> set ) {
	std::string comp;
	for (int n = 0; n < set.size(); n++) {
		comp += set[n] + " ";
	}
	return comp;
}


/*
G_SortFitness
sort fitness in descending order (hi->lo)
*/
std::vector<CSet> G_SortFitness( std::vector<CSet> population ) {

	std::vector<CSet> sorted = population;

	while (!UTIL_CheckSort(sorted)) { // While not sorted
		for (int i = 0; i < sorted.size(); i++) {
			if (i + 1 <= sorted.size()) { // Check if in bounds
				if (sorted[i].fitness > sorted[i + 1].fitness) {
					CSet temp; // Swap
					temp = sorted[i];
					sorted[i] = sorted[i + 1];
					sorted[i + 1] = temp;
				}
			}
		}
	}
	return sorted;
}

/*
G_GetPopulation
web scrape and return new population
*/
std::vector<CSet> G_GetPopulation( std::string keyword ) {
	std::vector<CSet> population;

	G_WriteFile("F:/" + KEYWORD_BUFFER, keyword); // Write keyword to file

	std::string command = "python F:/C++Project/sa1/Debug/mod/mod_scrape.py"; // Change later
	system(command.c_str());
	std::string content = G_ReadFile("F:/" + SCRAPE_BUFFER); // Read from scrape.txt file

	while (UTIL_GetScrape().length() == 0) { continue; }

	for (int c = 0; c < CSET_INSTANCES; c++) { // Make 10 class instances
		CSet* instance = new CSet();

		for (int n = 0; n < SENTENCE_IN_SET; n++) { // Create n amount of sentences in class
			std::string newSentence;
			for (long i = 0; i < content.length(); i++) { // Foreach char in content
				if (content[i] == '.') {
					instance->setInfo.push_back(newSentence); // Add to instance setInfo
					newSentence.clear(); // clear
				}
				else {
					newSentence += content[i]; // Add to new sentence
				}
			}
		}
		population.push_back(*instance); // Add instance to population
		instance = new CSet(); // Create new instance
	}
	return population;
}


/*
G_ReadFile
return text from file
*/
std::string G_ReadFile( std::string path ) {
	try {
		std::ifstream file(path);
		std::string   content;

		std::getline(file, content);
		file.close();
		return content;
	}
	catch(std::exception &e){
		G_MessageHandler(MSG_ERROR, UTIL_charTolpsz(e.what()));
	}

	return NULL;
}


/*
G_WriteFile
write contents to file
*/
bool G_WriteFile( std::string path, std::string content ) {
	try {
		std::ofstream file;

		file.open(path);
		file.clear();
		file << content;
		file.close();
		return true;
	}
	catch (std::exception &e) {
		G_MessageHandler(MSG_ERROR, UTIL_charTolpsz(e.what()));
	}
	return false;
}


/*
G_PlagarismCheck
send data to plagarism check module
*/
float G_PlagarismCheck( std::string compiledSet ) {
	try {
		UTIL_ClearFitness();

		std::ofstream set;
		set.open("F:/" + SCRAPE_BUFFER);
		set.clear();
		set << compiledSet;
		set.close();

		std::string command = "python F:/C++Project/sa1/Debug/mod/mod_plagarism.py"; //MODULE_PATH[MOD_PLAGARISM];
		system(command.c_str()); // Run plagarism script

		while (UTIL_GetFitness().length() == 0) { continue; } // Check if fitness file is empty

		// Get string to fraction
		std::string strFitness = UTIL_GetFitness();
		std::string digitBuffer;

		digitBuffer += strFitness[0];
		if (strFitness.length() > 2) { // Only add single digit if percent under 10%
			digitBuffer += strFitness[1];
		}

		float f_digit = atoi(digitBuffer.c_str());

		f_digit = f_digit / 100; // Get as fraction

		return f_digit;
	}
	catch (std::exception &e)
	{ 
		G_MessageHandler(MSG_ERROR, UTIL_charTolpsz(e.what()));
	}

	return 1;
}


/*
G_WinInit
Initialize window elements
*/
void G_WinInit() {
	SetWindowPos(*parentWin, NULL, 0, 0, 700, 500, NULL);
	G_CreateWidget(TYPE_STC,   CVec2D(25, 25),  CVec2D(90, 25),   L"KEYWORD:");
	G_CreateWidget(TYPE_TXTBX, CVec2D(125, 25), CVec2D(90, 25),   L"");
	G_CreateWidget(TYPE_BTN,   CVec2D(25, 75),  CVec2D(120, 25),  L"GENERATE");
	G_CreateWidget(TYPE_STC,   CVec2D(25, 175), CVec2D(625, 25),  L"CONSOLE");
	G_CreateWidget(TYPE_TXTBX, CVec2D(25, 200), CVec2D(625, 250), L"LOCK");
	G_CreateWidget(TYPE_STC,   CVec2D(500, 25), CVec2D(90, 25),   L"ESSAY SIZE:");
	G_CreateWidget(TYPE_STC,   CVec2D(500, 75), CVec2D(90, 50),   L"PEAK FITNESS:");
}