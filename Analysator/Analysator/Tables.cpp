#include "tables.h"

std::unordered_map<std::string, int> TW = {
	{"end", 1},
	{"begin", 2},
	{"var", 3},
	{"if", 4},
	{"then", 5},
	{"else", 6},
	{"for", 7},
	{"do", 8},
	{"while", 9},
	{"next", 10},
	{"enter", 11},
	{"displ", 12},
	{"true", 13},
	{"false", 14},
	{"int", 15},
	{"bool", 16},
	{"float", 17}
};

std::unordered_map<std::string, int> TL = {
	{"NE", 1},
	{"EQ", 2},
	{"LT", 3},
	{"LE", 4},
	{"GT", 5},
	{"GE", 6},
	{"plus", 7},
	{"min", 8},
	{"or", 9},
	{"mult", 10},
	{"assign", 11},
	{"div", 12},
	{"and", 13},
	{"~", 14},
	{";", 15},
	{",", 16},
	{":", 17},
	{"+", 18},
	{"-", 19},
	{"[", 20},
	{"]", 21},
	{" ", 22},
	{"\n", 23}
};

std::unordered_map<std::string, int> TN;
std::unordered_map<std::string, int> TI;