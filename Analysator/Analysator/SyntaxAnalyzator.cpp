#include <iostream>

#include "syntax.h"


void PROGRAMM() {
	gl();
	if (EQ("begin")) {
		do {
			gl();
			if (EQ("var")) {
				DESC();
			}
			else if (ID() || EQ("[") || EQ("if") || EQ("for") || EQ("while") || EQ("enter") || EQ("displ")) {
				OPERATOR();
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
}

void DESC() {
	if (EQ("var")) {
		gl();
		if (ID()) {
			while (ID()) {
				gl();
				while (EQ(",")) {
					gl();
					if (ID()) {
						gl();
					}
					else {
						throw std::runtime_error("Description expected identifier after ','");
					}
				}
				if (EQ(":")) {
					gl();
					TYPE();
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
}

void TYPE() {
	if (EQ("int") || EQ("float") || EQ("bool")) {
		gl();
	}
	else {
		throw std::runtime_error("Type expected ('int' || 'float' || 'bool')");
	}
}

void OPERATOR() {
	if (ID()) {
		ASSIGN();
	}
	else if (EQ("[")) {
		COMOPERATOR();
	}
	else if (EQ("if")) {
		IF();
	}
	else if (EQ("for")) {
		FOR();
	}
	else if (EQ("while")) {
		WHILE();
	}
	else if (EQ("enter")) {
		ENTER();
	}
	else if (EQ("displ")) {
		DISPLAY();
	}
	else {
		throw std::runtime_error("Operator expected (identifier || '[' || 'if' || 'for' || 'while' || 'enter' || 'displ'");
	}
}

void COMOPERATOR() {
	if (EQ("[")) {
		gl();
		OPERATOR();
		while (EQ(":") || EQ("\n")) {
			gl();
			OPERATOR();
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
}

void ASSIGN() {
	if (ID()) {
		gl();
		if (EQ("assign")) {
			gl();
			EXPRESSION();
		}
		else {
			throw std::runtime_error("Assign expected 'assign' after identifier");
		}
	}
	else {
		throw std::runtime_error("Assign expected identifier");
	}
}

void IF() {
	if (EQ("if")) {
		gl();
		EXPRESSION();
		if (EQ("then")) {
			gl();
			OPERATOR();
			if (EQ("else")) {
				gl();
				OPERATOR();
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
}

void FOR() {
	if (EQ("for")) {
		gl();
		ASSIGN();
		if (EQ("val")) {
			gl();
			EXPRESSION();
			if (EQ("do")) {
				gl();
				OPERATOR();
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
}

void WHILE() {
	if (EQ("while")) {
		gl();
		EXPRESSION();
		if (EQ("do")) {
			gl();
			OPERATOR();
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
}

void ENTER() {
	if (EQ("enter")) {
		gl();
		if (ID()) {
			gl();
			while (EQ(" ")) {
				gl();
				if (ID()) {
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
}

void DISPLAY() {
	if (EQ("displ")) {
		gl();
		EXPRESSION();
		while (EQ(",")) {
			gl();
			EXPRESSION();
		}

	}
	else {
		throw std::runtime_error("Display-operator expected 'displ'");
	}
}

void EXPRESSION() {
	OPERAND();
	while (EQ("GT") || EQ("GE") || EQ("NE") || EQ("EQ") || EQ("LT") || EQ("LE")) {
		gl();
		OPERAND();
	}
}

void OPERAND() {
	TERM();
	while (EQ("plus") || EQ("min") || EQ("or")) {
		gl();
		TERM();
	}
}

void TERM() {
	FACTOR();
	while (EQ("mult") || EQ("div") || EQ("and")) {
		gl();
		FACTOR();
	}
}

void FACTOR() {
	if (ID() || NUM() || (EQ("true")) || EQ("false")) {
		gl();
	}

	else if (EQ("~")) {
		gl();
		FACTOR();
	}

	else if (EQ("(")) {
		gl();
		EXPRESSION();
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
}

/*bool syntaxScan() {
	PROGRAMM();
}*/


