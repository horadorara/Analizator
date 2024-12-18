#pragma once
#include "lexeme.h"
#include <memory>
#include <unordered_map>
#include <stdexcept>

extern std::string LEX;

struct Node {
	std::string node;
	std::string value;
	std::string type;
	std::vector<std::shared_ptr<Node>> children;

	/*Node(const std::string& type, const std::string& value = "")
		: type(type), value(value) {}*/

	Node(const std::string& node, const std::string& value = "", const std::string& type = "")
		: node(node), value(value), type(type) {}

};

struct Symbol {
	std::string type; // Тип переменной: "int", "float", "bool"
	bool initialized; // Флаг, указывающий, была ли переменная инициализирована

	Symbol() : type(""), initialized(false) {}

	Symbol(const std::string& type, bool initialized = false)
		: type(type), initialized(initialized) {}
};

extern std::unordered_map<std::string, Symbol> symbolTable;

void printSyntaxTree(const std::shared_ptr<Node>& node, int depth = 0);

void gl();
std::string currentToken();
bool EQ(std::string S);
bool ID();
bool NUM();
std::shared_ptr<Node> PROGRAMM();
std::shared_ptr<Node> DESC();
std::shared_ptr<Node> TYPE();
std::shared_ptr<Node> OPERATOR();
std::shared_ptr<Node> COMOPERATOR();
std::shared_ptr<Node> ASSIGN();
std::shared_ptr<Node> IF();
std::shared_ptr<Node> FOR();
std::shared_ptr<Node> WHILE();
std::shared_ptr<Node> ENTER();
std::shared_ptr<Node> DISPLAY();
std::shared_ptr<Node> EXPRESSION();
std::shared_ptr<Node> OPERAND();
std::shared_ptr<Node> TERM();
std::shared_ptr<Node> FACTOR();
std::string evaluateExpressionType(const std::shared_ptr<Node>& exprNode);