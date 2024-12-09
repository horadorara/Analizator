#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

struct Token {
	int number;
	int value;
};

std::vector<Token> tokens;

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
	{"false", 14}
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
	{"+", 11},
	{"-", 12},
	{"assign", 13},
	{"div", 14},
	{"and", 15},
	{"~", 16},
	{";", 17},
	{",", 18},
	{":", 19},
	{"[", 20},
	{"]", 21}
};


std::unordered_map<std::string, int> TN;
std::unordered_map<std::string, int> TI;

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

std::ifstream input("input.txt");
int z = 0;
char CH;
std::string S;

void gc() {
	if (input.get(CH))
		std::cout << "GC:" << CH << '\n';
	else
		CH = '\0';
}

void add() {
	S.push_back(CH);
	std::cout << "S:" << S << '\n';
}

void nill() {
	S.clear();
}

void out(int numb, int value) {
	tokens.push_back({ numb, value });
}

void outFile(const std::string &fileName) {
	std::ofstream tokensOut(fileName, std::ios::trunc);
	if (tokensOut.is_open()) {
		for (const auto& token : tokens) {
			tokensOut << token.number << ", " << token.value << std::endl;
		}
		tokensOut.close();
	}
	else
		std::cerr << "Error: can't open the file\n";
}

void put(std::unordered_map<std::string, int> &table) {
	auto got = table.find(S);
	if (got == table.end()) {
		z = table.size() + 1;
		table[S] = z;
	}
	else
		z = got->second;
}

void look(std::unordered_map<std::string, int> &table) {
	auto got = table.find(S);
	if (got != table.end())
		z = got->second;
	else
		z = 0;
}

bool let() {
	if (std::isalpha(CH))
		return true;
	else
		return false;
}

bool digit() {
	if (std::isdigit(CH))
		return true;
	else
		return false;
}

int main(int argc, char* argv[]) {
	states CS;
	CS = H;
	gc();
	while (CS != V) {
		switch (CS) {
		case H:

			while (CH == ' ' || CH == '\n')
				gc();

			if (let()) {
				nill();
				add();
				gc();
				CS = I;
			}

			else if (CH <= '1') {
				nill();
				add();
				gc();
				CS = N2;
			}

			else if (CH >= '2' && CH <= '7') {
				nill();
				add();
				gc();
				CS = N8;
			}

			else if (CH >= '8') {
				nill();
				add();
				gc();
				CS = N10;
			}

			else if (CH == '.') {
				nill();
				add();
				gc();
				CS = NR;
			}

			else if (CH == '[') {
				gc();
				CS = C;
			} 
			
			else {
				nill();
				add();
				gc();
				look(TL);
				if (z != 0) {
					out(2, z);
					CS = H;
				} else
					CS = ER;

			}
			break;

		case I:
			while (let() || digit()) {
				add();
				if (CH != '\n')
					gc();
			}
			look(TW);
			if (z != 0) {
				if (z == 1) {
					out(1, 1);
					CS = V;
				} else {
					out(1, z);
					CS = H;
				}
			} else {
				look(TL);
				if (z != 0) {
					out(2, z);
					CS = H;
				}
				else {
					put(TI);
					out(4, z);
					CS = H;
				}
			}
			break;

		case N2:

			while (CH == '0' || CH == '1') {
				add();
				gc();
			}

			if (CH >= '2' && CH <= '7') {
				add();
				gc();
				CS = N8;
			}

			else if (tolower(CH) == 'o') {
				add();
				gc();
				CS = _N8;
			}

			else if (CH >= '8') {
				add();
				gc();
				CS = N10;
			}

			else if (tolower(CH) == 'd') {
				add();
				gc();
				CS = _N10;
			}

			else if (tolower(CH) == 'a' || tolower(CH) == 'c' || tolower(CH) == 'f') {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (tolower(CH) == 'e') {
				add();
				gc();
				CS = EXP;
			}

			else if (CH == '.') {
				add();
				gc();
				CS = NR;
			}

			else if (tolower(CH) == 'b') {
				add();
				gc();
				CS = _N2;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;
			break;

		case _N2:

			if (tolower(CH) == 'a' || tolower(CH) == 'b' ||
				tolower(CH) == 'c' || tolower(CH) == 'd' ||
				tolower(CH) == 'e' || tolower(CH) == 'f' || digit()) {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;
			break;

		case N8:

			while (CH >= '0' && CH <= '7') {
				add();
				gc();
			}

			if (CH >= '8') {
				add();
				gc();
				CS = N10;
			}

			else if (tolower(CH) == 'd') {
				add();
				gc();
				CS = _N10;
			}

			else if (tolower(CH) == 'a' || tolower(CH) == 'b' || tolower(CH) == 'c' || tolower(CH) == 'f') {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (tolower(CH) == 'e') {
				add();
				gc();
				CS = EXP;
			}

			else if (CH == '.') {
				add();
				gc();
				CS = NR;
			}

			else if (tolower(CH) == 'o') {
				add();
				gc();
				CS = _N8;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;

			break;

		case _N8:

			if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;

			break;

		case N10:

			while (digit()) {
				add();
				gc();
			}

			if (tolower(CH) == 'a' || tolower(CH) == 'b' || tolower(CH) == 'c' || tolower(CH) == 'f') {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (tolower(CH) == 'e') {
				add();
				gc();
				CS = EXP;
			}

			else if (CH == '.') {
				add();
				gc();
				CS = NR;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
			}

			else if (tolower(CH) == 'd') {
				add();
				gc();
				CS = _N10;
			}

			else
				CS = ER;

			break;

		case _N10:

			if (tolower(CH) == 'a' || tolower(CH) == 'b' ||
				tolower(CH) == 'c' || tolower(CH) == 'd' ||
				tolower(CH) == 'e' || tolower(CH) == 'f' || digit()) {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
			}

			else
				CS = ER;

			break;

		case N16:

			while (digit() || CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
			}

			if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;
			break;

		case _N16:

			 if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;
			break;

		case NR:
			while (digit()) {
				add();
				gc();
			}

			if (tolower(CH) == 'e') {
				add();
				gc();
				CS = EXPR;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;
			break;

		case EXP:

			if (CH == '+' || CH == '-') {
				add();
				gc();
				CS = EXPN1;
			}

			else if (digit()) {
				add();
				gc();
				CS = EXPN2;
			}

			else if (CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else
				CS = ER;
			
			break;

		case EXPR:

			if (CH == '+' || CH == '-' || digit()) {
				add();
				gc();
				CS = EXPN1;
			}

			else
				CS = ER;
			
			break;

		case EXPN1:

			while (digit()) {
				add();
				gc();
			}

			if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;

			break;

		case EXPN2:

			while (digit()) {
				add();
				gc();
			}

			if (CH >= 'a' && CH <= 'f' || CH >= 'A' && CH <= 'F') {
				add();
				gc();
				CS = N16;
			}

			else if (tolower(CH) == 'h') {
				add();
				gc();
				CS = _N16;
			}

			else if (CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else
				CS = ER;

			break;

		case C:

			while (CH != ']') {
				gc();
			}

			if (CH == ']') {
				gc();
				CS = C1;
			}

			break;

		case C1:

			gc();
			CS = H;
			break;

		case V:
			return true;

		case ER:
			std::cout << "ERROR";
			return false;
		}
		outFile("tokens.txt");
	}
	return 0;
}


