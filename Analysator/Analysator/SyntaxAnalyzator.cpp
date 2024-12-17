#include <iostream>

#include "syntax.h"

/*struct Node {
	std::string type;
	std::string value;
	std::vector<std::shared_ptr<Node>> children;

	Node(const std::string& type, const std::string& value = "")
		: type(type), value(value) {}
};*/

void printSyntaxTree(const std::shared_ptr<Node>& node, int depth) {
	for (int i = 0; i < depth; ++i) {
		std::cout << "  ";
	}
	std::cout << node->type;
	if (!node->value.empty()) {
		std::cout << ": " << node->value;
	}
	std::cout << "\n";
	for (const auto& child : node->children) {
		printSyntaxTree(child, depth + 1);
	}
}



std::shared_ptr<Node> PROGRAMM() {
	auto root = std::make_shared<Node>("PROGRAMM");
	gl();
	if (EQ("begin")) {
		do {
			gl();
			if (EQ("var")) {
				root->children.push_back(DESC());
			}
			else if (ID() || EQ("[") || EQ("if") || EQ("for") || EQ("while") || EQ("enter") || EQ("displ")) {
				root->children.push_back(OPERATOR());
			}
			else {
				throw std::runtime_error("Expected description or operator after 'begin'");
			}
		} while (EQ(";"));

		if (!EQ("end")) {
			throw std::runtime_error("Program must end with 'end'");
		}
	}
	else {
		throw std::runtime_error("Program start with 'begin'");
	}
	return root;
}

std::shared_ptr<Node> DESC() {
	auto descNode = std::make_shared<Node>("DESC");
	if (EQ("var")) {
		gl();
		if (ID()) {
			while (ID()) {
				descNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
				gl();
				while (EQ(",")) {
					gl();
					if (ID()) {
						descNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
						gl();
					}
					else {
						throw std::runtime_error("Description expected identifier after ','");
					}
				}
				if (EQ(":")) {
					gl();
					descNode->children.push_back(TYPE());
					if (EQ(";")) {
						gl();
					}
					else {
						throw std::runtime_error("Description expected ';' after type");
					}
				}
				else {
					throw std::runtime_error("Description expected ':' after identefier");
				}
			}
		}
		else {
			gl();
		}
	}
	else {
		throw std::runtime_error("Description expected 'var'");
	}
	return descNode;
}

std::shared_ptr<Node> TYPE() {
	auto typeNode = std::make_shared<Node>("TYPE");
	if (EQ("int")) {
		typeNode->children.push_back(std::make_shared<Node>("INT"));
		gl();
	}

	else if (EQ("float")) {
		typeNode->children.push_back(std::make_shared<Node>("FLOAT"));
		gl();
	}

	else if (EQ("bool")) {
		typeNode->children.push_back(std::make_shared<Node>("BOOL"));
		gl();
	}
	else {
		throw std::runtime_error("Type expected ('int' || 'float' || 'bool')");
	}
	return typeNode;
}

std::shared_ptr<Node> OPERATOR() {
	auto operNode = std::make_shared<Node>("OPERATOR");
	if (ID()) {
		operNode->children.push_back(ASSIGN());
	}
	else if (EQ("[")) {
		operNode->children.push_back(COMOPERATOR());
	}
	else if (EQ("if")) {
		operNode->children.push_back(IF());
	}
	else if (EQ("for")) {
		operNode->children.push_back(FOR());
	}
	else if (EQ("while")) {
		operNode->children.push_back(WHILE());
	}
	else if (EQ("enter")) {
		operNode->children.push_back(ENTER());
	}
	else if (EQ("displ")) {
		operNode->children.push_back(DISPLAY());
	}
	else {
		throw std::runtime_error("Operator expected (identifier || '[' || 'if' || 'for' || 'while' || 'enter' || 'displ'");
	}
	return operNode;
}

std::shared_ptr<Node> COMOPERATOR() {
	auto comoperNode = std::make_shared<Node>("COMOPERATOR");
	if (EQ("[")) {
		gl();
		comoperNode->children.push_back(OPERATOR());
		while (EQ(":") || EQ("\n")) {
			gl();
			comoperNode->children.push_back(OPERATOR());
		}
		if (EQ("]")) {
			gl();
		}
		else {
			throw std::runtime_error("Compound operator expected ']' after operator");
		}
	}
	else {
		throw std::runtime_error("Compound operator expected '['");
	}

	return comoperNode;
}

std::shared_ptr<Node> ASSIGN() {
	auto assignNode = std::make_shared<Node>("ASSIGN");
	if (ID()) {
		assignNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
		gl();
		if (EQ("assign")) {
			gl();
			assignNode->children.push_back(EXPRESSION());
		}
		else {
			throw std::runtime_error("Assign expected 'assign' after identifier");
		}
	}
	else {
		throw std::runtime_error("Assign expected identifier");
	}

	return assignNode;
}

std::shared_ptr<Node> IF() {
	auto ifNode = std::make_shared<Node>("IF");
	if (EQ("if")) {
		gl();
		ifNode->children.push_back(EXPRESSION());
		if (EQ("then")) {
			gl();
			ifNode->children.push_back(OPERATOR());
			if (EQ("else")) {
				gl();
				ifNode->children.push_back(OPERATOR());
				if (EQ("end")) {
					gl();
				}
				else {
					throw std::runtime_error("If-operator expected 'end' after operator");
				}
			}
			else if (EQ("end")) {
				gl();
			}
			else {
				throw std::runtime_error("If-operator expected 'else' or 'end' after operator");
			}
		}
		else {
			throw std::runtime_error("If-operator expected 'then' after expression");
		}
	}
	else {
		throw std::runtime_error("If-operator excepted 'if' ");
	}

	return ifNode;
}

std::shared_ptr<Node> FOR() {
	auto forNode = std::make_shared<Node>("FOR");
	if (EQ("for")) {
		gl();
		forNode->children.push_back(ASSIGN());
		if (EQ("val")) {
			gl();
			forNode->children.push_back(EXPRESSION());
			if (EQ("do")) {
				gl();
				forNode->children.push_back(OPERATOR());
			}
			else {
				throw std::runtime_error("For-operator expected 'do' after expression");
			}
		}
		else {
			throw std::runtime_error("For-operator expected 'val' after assign");
		}
	}
	else {
		throw std::runtime_error("For-operator expected 'for'");
	}

	return forNode;
}

std::shared_ptr<Node> WHILE() {
	auto whileNode = std::make_shared<Node>("WHILE");
	if (EQ("while")) {
		gl();
		whileNode->children.push_back(EXPRESSION());
		if (EQ("do")) {
			gl();
			whileNode->children.push_back(OPERATOR());
			if (EQ("next")) {
				gl();
			}
			else {
				throw std::runtime_error("While-operator expected 'next' after operator");
			}
		}
		else {
			throw std::runtime_error("While-operator expected 'do' after expression");
		}
	}
	else {
		throw std::runtime_error("While-operator expected 'while'");
	}

	return whileNode;
}

std::shared_ptr<Node> ENTER() {
	auto enterNode = std::make_shared<Node>("ENTER");
	if (EQ("enter")) {
		gl();
		if (ID()) {
			enterNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
			gl();
			while (EQ(" ")) {
				gl();
				if (ID()) {
					enterNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
					gl();
				}
				else {
					throw std::runtime_error("Enter-operator expected identifier after ' '");
				}
			}
		}
		else {
			throw std::runtime_error("Enter-operator expected identifier after 'enter");
		}
	}
	else {
		throw std::runtime_error("Enter-operator expected 'enter'");
	}

	return enterNode;
}

std::shared_ptr<Node> DISPLAY() {
	auto displNode = std::make_shared<Node>("DISPLAY");
	if (EQ("displ")) {
		gl();
		displNode->children.push_back(EXPRESSION());
		while (EQ(",")) {
			gl();
			displNode->children.push_back(EXPRESSION());
		}

	}
	else {
		throw std::runtime_error("Display-operator expected 'displ'");
	}
	
	return displNode;
}

std::shared_ptr<Node> EXPRESSION() {
	auto expressNode = std::make_shared<Node>("EXPRESSION");
	expressNode->children.push_back(OPERAND());
	while (EQ("GT") || EQ("GE") || EQ("NE") || EQ("EQ") || EQ("LT") || EQ("LE")) {
		gl();
		expressNode->children.push_back(OPERAND());
	}

	return expressNode;
}

std::shared_ptr<Node> OPERAND() {
	auto operandNode = std::make_shared<Node>("OPERAND");
	operandNode->children.push_back(TERM());
	while (EQ("plus") || EQ("min") || EQ("or")) {
		gl();
		operandNode->children.push_back(TERM());
	}

	return operandNode;
}

std::shared_ptr<Node> TERM() {
	auto termNode = std::make_shared<Node>("TERM");
	termNode->children.push_back(FACTOR());
	while (EQ("mult") || EQ("div") || EQ("and")) {
		gl();
		termNode->children.push_back(FACTOR());
	}

	return termNode;
}

std::shared_ptr<Node> FACTOR() {
	auto factorNode = std::make_shared<Node>("FACTOR");
	if (ID()) {
		factorNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
		gl();
	}

	else if (NUM()) {
		factorNode->children.push_back(std::make_shared<Node>("NUMBER", LEX));
		gl();
	}

	else if (EQ("true") || EQ("false")) {
		factorNode->children.push_back(std::make_shared<Node>("LOGICALCONST", LEX));
		gl();
	}

	else if (EQ("~")) {
		factorNode->children.push_back(std::make_shared<Node>("NOTOPERATION", LEX));
		gl();
		factorNode->children.push_back(FACTOR());
	}

	else if (EQ("(")) {
		gl();
		factorNode->children.push_back(EXPRESSION());
		if (EQ(")")) {
			gl();
		}
		else {
			throw std::runtime_error("Factor expected ')' after expression");
		}
	}

	else {
		throw std::runtime_error("Factor expected (identifier || number || 'true' || 'false' || '~' || '('");
	}

	return factorNode;
}