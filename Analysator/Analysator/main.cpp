#include "lex.h"
#include "tables.h"
#include "syntax-semantic.h"

#include <chrono>

int main() {
	//auto start = std::chrono::high_resolution_clock::now();
	try {
		if (!lexScan()) {
			throw std::runtime_error("Lexical analysis failed");
		}
		outFile("tokens.txt");
		std::cout << "------Lex Analyze Complete!------" << std::endl << std::endl;

		std::cout << "Syntax-Semantic Analyze" << std::endl;
		auto syntaxTree = PROGRAMM();
		printSyntaxTree(syntaxTree);
		std::cout << "-----Syntax-Semantic Complete!-----" << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown error occurred!" << std::endl;
		return 1;
	}

	/*auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Execution time: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms\n";*/
	
	return 0;
}