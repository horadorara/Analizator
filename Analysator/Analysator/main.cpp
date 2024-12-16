#include "lex.h"
#include "tables.h"
#include "syntax.h"

int main() {
	try {
		if (!lexScan()) {
			throw std::runtime_error("Lexical analysis failed");
		}
		outFile("tokens.txt");
		std::cout << "Lex Analyse Complete!" << std::endl;

		PROGRAMM();
		std::cout << "Syntax analysis complete!" << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1; // Завершаем программу с кодом ошибки
	}
	catch (...) {
		std::cerr << "Unknown error occurred!" << std::endl;
		return 1;
	}
	
	return 0;
}