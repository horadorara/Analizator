#ifndef LEX
#define LEX

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

enum states {
	H,
	I,
	C,
	C1,
	N2,
	_N2,
	N8,
	_N8,
	N10,
	_N10,
	N16,
	_N16,
	NR,
	EXP,
	EXPR,
	EXPN1,
	EXPN2,
	ER,
	V
};

enum LexError {
	UnknownSymbole,
	ExpectedEndCommentary,
	InvalidNumber,
};

struct ErrorInfo {
	LexError type;
	int line;
	int position;
	std::string message;
};

extern std::vector<ErrorInfo> errors;

void logError(const ErrorInfo& error);

void handleError(LexError type, int line = -1, int position = -1);

extern int z;
extern char CH;
extern std::string S;
extern int currentLine;
extern int currentPosition;

void gc();

void add();

void nill();
void out(int numb, int value);

void outFile(const std::string& fileName);

void put(std::unordered_map<std::string, int>& table);

void look(std::unordered_map<std::string, int>& table);

bool let();

bool digit();

bool checkTL();

bool lexScan();

#endif