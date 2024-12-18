#include <iostream>
#include <sstream>

#include "tables.h"
#include "syntax-semantic.h"

int currentLexIndex = 0;
Token currentLex;
std::string LEX;

void gl() {
	if (currentLexIndex >= tokens.size()) {
		return;
	}
	currentLex = tokens[currentLexIndex];
	std::unordered_map<std::string, int> table;
	switch (currentLex.number) {
	case 1:
		table = TW;
		break;
	case 2:
		table = TL;
		break;
	case 3:
		table = TN;
		break;
	case 4:
		table = TI;
		break;
	}

	for (const auto& entry : table) {
		if (entry.second == currentLex.value) {
			LEX = entry.first;
			//std::cout << "Value of table:" << LEX << std::endl;
			break;
		}
	}
	currentLexIndex++;

}

bool EQ(std::string S) {
	return LEX == S;
}

bool ID() {
	if (TI.find(LEX) != TI.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool NUM() {
	if (TN.find(LEX) != TN.end()) {
		return true;
	}
	else {
		return false;
	}
}