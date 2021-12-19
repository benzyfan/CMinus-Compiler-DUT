#pragma once
//Ω‚ Õ≥Ã–Ú
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "WordAnalyzer.h"
#include "GrammerAnalyzer.h"
using namespace std;

class Explain {

public:
	GrammerAnalyze& grammerresult;
	Explain(GrammerAnalyze&);
	void Valueout(int, int);
	void outing(int posi);
};