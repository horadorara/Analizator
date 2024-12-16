#include "lex.h"
#include "tables.h"
#include "lexeme.h"

int z = 0;
char CH;
std::string S;
std::ifstream input("input.txt");
int currentLine = 1;
int currentPosition = 0;
std::vector<ErrorInfo> errors;
std::vector<Token> tokens;

// Функция для записи ошибок в файл
void logError(const ErrorInfo& error) {
	std::ofstream errorLog("error_log.txt", std::ios::app);
	if (errorLog.is_open()) {
		errorLog << "Error: " << error.message
			<< " at line: " << error.line
			<< ", position: " << error.position << std::endl;
		errorLog.close();
	}
	else {
		std::cerr << "Error: can't open error log file.\n";
	}
}

// Функция для обработки ошибок
void handleError(LexError type, int line, int position) {
	std::map<LexError, std::string> errorMessages = {
		{ UnknownSymbole, "Unknown symbol encountered" },
		{ ExpectedEndCommentary, "Expected end commentary ']'" },
		{ InvalidNumber, "Invalid number form"}
	};

	// Формируем информацию об ошибке
	ErrorInfo error = { type, line, position, errorMessages[type] };
	errors.push_back(error);
	logError(error);

	// Выводим сообщение об ошибке на экран
	std::cerr << "Error: " << error.message;
	if (line != -1)
		std::cerr << " at line: " << line;
	if (position != -1)
		std::cerr << ", position: " << position << std::endl;

	std::cerr << std::endl;
}

void gc() {
	if (input.get(CH)) {
		currentPosition++;
		//std::cout << "GC:" << CH << " position:" << currentPosition << '\n';
	}
	else
		CH = '\0';
}

void add() {
	S.push_back(CH);
	//std::cout << "S:" << S << '\n';
}

void nill() {
	S.clear();
}

void out(int numb, int value) {
	std::cout << "line:" << currentLine << std::endl;;
	std::cout << "Token(" << numb << ", " << value << ") Text:'" << S << "' Line:" << currentLine << std::endl << std::endl;
	tokens.push_back({ numb, value });
}

void outFile(const std::string& fileName) {
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

void put(std::unordered_map<std::string, int>& table) {
	auto got = table.find(S);
	if (got == table.end()) {
		z = table.size() + 1;
		table[S] = z;
	}
	else
		z = got->second;
}

void look(std::unordered_map<std::string, int>& table) {
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

bool checkTL() {
	std::string str;
	str.push_back(CH);
	auto got = TL.find(str);
	if (got != TL.end())
		return true;
	else
		return false;
}