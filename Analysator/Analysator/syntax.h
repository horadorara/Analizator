#pragma once
#include "lexeme.h"
#include <memory>

extern std::string LEX;

struct Node {
	std::string type;
	std::string value;
	std::vector<std::shared_ptr<Node>> children;

	Node(const std::string& type, const std::string& value = "")
		: type(type), value(value) {}
};

void printSyntaxTree(const std::shared_ptr<Node>& node, int depth = 0);

void gl();
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