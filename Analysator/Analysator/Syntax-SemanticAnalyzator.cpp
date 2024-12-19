#include <iostream>

#include "syntax-semantic.h"

std::unordered_map<std::string, Symbol> symbolTable;

void printSyntaxTree(const std::shared_ptr<Node>& node, int depth) {
	for (int i = 0; i < depth; ++i) {
		std::cout << "  ";
	}
	std::cout << node->node;
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
				std::string varName = LEX;
				if (symbolTable.find(varName) != symbolTable.end()) {
					throw std::runtime_error("Variable '" + varName + "' already declared");
				}
				descNode->children.push_back(std::make_shared<Node>("IDENTIFIER", varName));
				gl();
				while (EQ(",")) {
					gl();
					if (ID()) {
						varName = LEX;
						if (symbolTable.find(varName) != symbolTable.end()) {
							throw std::runtime_error("Variable '" + varName + "' already declared");
						}
						descNode->children.push_back(std::make_shared<Node>("IDENTIFIER", varName));
						gl();
					}
					else {
						throw std::runtime_error("Description expected identifier after ','");
					}
				}
				if (EQ(":")) {
					gl();
					auto typeNode = TYPE();
					std::string varType = typeNode->value;
					for (auto& child : descNode->children) {
						symbolTable[child->value] = Symbol(varType);
					}
					/*for (const auto& pair : symbolTable) {
						const std::string& key = pair.first;
						const Symbol& symbol = pair.second;
						std::cout << "Key: " << key << ", Type: " << symbol.type
							<< ", Initialized: " << (symbol.initialized ? "Yes" : "No") << "\n";
					}*/
					descNode->children.push_back(typeNode);
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
		typeNode->value.append("int");
		gl();
	}

	else if (EQ("float")) {
		typeNode->value.append("float");
		gl();
	}

	else if (EQ("bool")) {
		typeNode->value.append("bool");
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
		std::string varName = LEX;
		if (symbolTable.find(varName) == symbolTable.end()) {
			throw std::runtime_error("Variable '" + varName + "' not declared");
		}
		assignNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
		gl();
		if (EQ("assign")) {
			gl();
			auto exprNode = EXPRESSION();
			assignNode->children.push_back(exprNode);

			std::string varType = symbolTable[varName].type;
			std::string exprType = exprNode->type;
			if (varType != exprType) {
				throw std::runtime_error("Type mismatch: cannot assign " + exprType + " to " + varType);
			}

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
		auto exprNode = EXPRESSION();
		ifNode->children.push_back(exprNode);

		std::string exprType = exprNode->type;
		if (exprType != "bool") {
			throw std::runtime_error("Type mismatch: cannot assign " + exprType + " to " + "IF");
		}
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
			auto exprNode = EXPRESSION();
			forNode->children.push_back(exprNode);

			std::string exprType = exprNode->type;
			if (exprType != "bool") {
				throw std::runtime_error("Type mismatch: cannot assign " + exprType + " to " + "FOR");
			}
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

		auto exprNode = EXPRESSION();
		whileNode->children.push_back(exprNode);

		std::string exprType = exprNode->type;
		if (exprType != "bool") {
			throw std::runtime_error("Type mismatch: cannot assign " + exprType + " to " + "WHILE");
		}
		
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
			std::string varName = LEX;
			if (symbolTable.find(varName) == symbolTable.end()) {
				throw std::runtime_error("Variable '" + varName + "' not declared");
			}
			enterNode->children.push_back(std::make_shared<Node>("IDENTIFIER", LEX));
			gl();
			while (EQ(" ")) {
				gl();
				if (ID()) {
					std::string varName = LEX;
					if (symbolTable.find(varName) == symbolTable.end()) {
						throw std::runtime_error("Variable '" + varName + "' not declared");
					}
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
	auto leftOperand = OPERAND();
	expressNode->children.push_back(leftOperand);
	expressNode->type = leftOperand->type;

	while (EQ("GT") || EQ("GE") || EQ("NE") || EQ("EQ") || EQ("LT") || EQ("LE")) {
		if (EQ("GT")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "GT"));
		}

		else if (EQ("GE")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "GE"));
		}

		else if (EQ("NE")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "NE"));
		}

		else if (EQ("EQ")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "EQ"));
		}

		else if (EQ("LT")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "LT"));
		}

		else if (EQ("LE")) {
			expressNode->children.push_back(std::make_shared<Node>("OPERATOR", "LE"));
		}
		expressNode->type = "bool";
		gl();
		auto rightOperand = OPERAND();
		expressNode->children.push_back(rightOperand);

		if (leftOperand->type != rightOperand->type) {
			throw std::runtime_error("Type mismatch in expression: '" +
				leftOperand->type + "' and '" + rightOperand->type + "'");
		}
	}

	return expressNode;
}

std::shared_ptr<Node> OPERAND() {
	auto operandNode = std::make_shared<Node>("OPERAND");
	auto leftTermNode = TERM();
	operandNode->type = leftTermNode->type;
	operandNode->children.push_back(leftTermNode);
	while (EQ("plus") || EQ("min") || EQ("or")) {
		if (EQ("plus")) {
			operandNode->children.push_back(std::make_shared<Node>("OPERATOR", "plus"));
		}

		else if (EQ("min")) {
			operandNode->children.push_back(std::make_shared<Node>("OPERATOR", "min"));
		}

		else if (EQ("or")) {
			operandNode->children.push_back(std::make_shared<Node>("OPERATOR", "or"));
		}
		gl();
		auto termNodeRight = TERM();
		operandNode->children.push_back(termNodeRight);

		if (leftTermNode->type != termNodeRight->type) {
			throw std::runtime_error("Type mismatch in operation: '" +
				 leftTermNode->type + "' and '" + termNodeRight->type + "'");
		}
	}

	return operandNode;
}

std::shared_ptr<Node> TERM() {
	auto termNode = std::make_shared<Node>("TERM");
	auto leftFactorNode = FACTOR();
	termNode->type = leftFactorNode->type;
	termNode->children.push_back(leftFactorNode);
	while (EQ("mult") || EQ("div") || EQ("and")) {
		if (EQ("mult")) {
			termNode->children.push_back(std::make_shared<Node>("OPERATOR", "mult"));
		}

		else if (EQ("div")) {
			termNode->children.push_back(std::make_shared<Node>("OPERATOR", "div"));
		}

		else if (EQ("and")) {
			termNode->children.push_back(std::make_shared<Node>("OPERATOR", "and"));
		}
		gl();
		auto rightFactorNode = FACTOR();
		termNode->children.push_back(rightFactorNode);

		if (leftFactorNode->type != rightFactorNode->type) {
			throw std::runtime_error("Type mismatch in operation: '" +
				leftFactorNode->type + "' and '" + rightFactorNode->type + "'");
		}

	}
	return termNode;
}

std::shared_ptr<Node> FACTOR() {
	auto factorNode = std::make_shared<Node>("FACTOR");
	if (ID()) {
		auto identifierNode = std::make_shared<Node>("IDENTIFIER", LEX);
		factorNode->children.push_back(identifierNode);
		std::string varName = LEX;
		if (symbolTable.find(varName) == symbolTable.end()) {
			throw std::runtime_error("Variable '" + varName + "' not declared");
		}
		factorNode->type = symbolTable[LEX].type;
		gl();
	}

	else if (NUM()) {
		auto numberNode = std::make_shared<Node>("NUMBER", LEX);
		factorNode->children.push_back(numberNode);
		if (numberNode->value.find('.') != std::string::npos || numberNode->value.find_first_of('Ee') != std::string::npos && std::tolower(factorNode->value.back() != 'h')) {
			numberNode->type.append("float");
		}
		else {
			numberNode->type.append("int");
		}

		factorNode->type = numberNode->type;
		gl();
	}

	else if (EQ("true") || EQ("false")) {
		factorNode->children.push_back(std::make_shared<Node>("LOGICALCONST", LEX));
		factorNode->type = "bool";
		gl();
	}

	else if (EQ("~")) {
		factorNode->children.push_back(std::make_shared<Node>("NOTOPERATION", LEX));
		gl();
		auto subFactor = FACTOR();
		factorNode->children.push_back(subFactor);
		factorNode->type = subFactor->type;
	}

	else if (EQ("(")) {
		gl();
		auto expressionNode = EXPRESSION();
		factorNode->children.push_back(expressionNode);
		factorNode->type = expressionNode->type;
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