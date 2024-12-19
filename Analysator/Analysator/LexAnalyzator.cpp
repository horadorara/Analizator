#include "lex.h"
#include "tables.h"
#include "lexeme.h"
#include <stack>

std::stack<char> bracketStack;
std::stack<std::string> ifStack;


bool lexScan() {
	states CS;
	CS = H;
	bool isEnter = false;
	bool unskipSpace = false;
	bool isComOper = false;
	bool isIf = false;
	gc();
	while (CS != V) {
		switch (CS) {
		case H:

			while (CH == ' ' || CH == '\n' || CH == '\t') {
				if (CH == '\n') {
					currentLine++;
					currentPosition = 0;
					if (isComOper) {
						unskipSpace = false;
						isEnter = false;
						nill();
						add();
						out(2, 23);
					}
					gc();
				}

				else if (CH == ' ') {
					if (unskipSpace) {
						nill();
						add();
						out(2, 22);
					}
					gc();
				}

				else {
					gc();
				}
			}

			if (let()) {
				nill();
				add();
				gc();
				CS = I;
			}

			else if (CH == '0' || CH == '1') {
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

			else if (CH == '8' || CH == '9') {
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

			else if (CH == '{') {
				nill();
				add();
				gc();
				CS = C;
			}

			else if (CH == ';') {
				nill();
				add();
				gc();
				out(2, 15);
				unskipSpace = false;
				isEnter = false;
				CS = H;
			}

			else if (CH == ':') {
				nill();
				add();
				gc();
				out(2, 17);
				if (isComOper) {
					unskipSpace = false;
					isEnter = false;
				}
			}

			else if (CH == '[') {
				nill();
				add();
				gc();
				out(2, 20);

				bracketStack.push('[');

				if (!isComOper) {
					isComOper = true;
				}
				CS = H;
			}

			else if (CH == ']') {
				nill();
				add();
				gc();
				out(2, 21);

				if (!isComOper) {
					isComOper = true;
				}if (!bracketStack.empty()) {

					bracketStack.pop();
				}
				else {
					throw std::runtime_error("Unmatched closing bracket ']' detected.");
				}

				if (bracketStack.empty()) {
					isComOper = false;
				}
				CS = H;
			}
			
			else {
				nill();
				add();
				look(TL);
				if (z != 0) {
					out(2, z);
					gc();
					CS = H;
				}
				else {
					handleError(UnknownSymbole, currentLine, currentPosition);
					CS = ER;
				}

			}
			break;

		case I:
			while (let() || digit()) {
				add();
				gc();
			}
			look(TW);
			if (z != 0) {
				if (z == 1) {
					out(1, 1);
					if (isIf) {
						if (!isIf) {
							isIf = true;
						}
						if (!ifStack.empty()) {
							ifStack.pop();
						}
						else {
							throw std::runtime_error("Unmatched closing if 'end' detected.");
						}

						if (ifStack.empty()) {
							isIf = false;
						}

						CS = H;
						//isIf = false;
					}
					else {
						std::cout.flush();
						CS = V;
					}
				}
				else if (z == 11) {
					out(1, 11);
					isEnter = true;
					CS = H;
				}

				else if (z == 4) {
					out(1, 4);
					ifStack.push("if");

					if (!isIf) {
						isIf = true;
					}
					//isIf = true;
					CS = H;

				}
				else {
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
					if (isEnter) {
						unskipSpace = true;
					}
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

			else if (CH == '8' || CH == '9') {
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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				CS = ER;
				handleError(InvalidNumber, currentLine, currentPosition);
			}
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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

			break;

		case N8:

			while (CH >= '0' && CH <= '7') {
				add();
				gc();
			}

			if (CH == '8' || CH == '9') {
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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

			break;

		case _N8:

			if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
			}

			else if (tolower(CH) == 'd') {
				add();
				gc();
				CS = _N10;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

			break;

		case _N16:

			 if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			 }

			 else {
				 handleError(InvalidNumber, currentLine, currentPosition);
				 CS = ER;
			 }

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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

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

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}
			
			break;

		case EXPR:

			if (CH == '+' || CH == '-' || digit()) {
				add();
				gc();
				CS = EXPN1;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}
			
			break;

		case EXPN1:

			while (digit()) {
				add();
				gc();
			}

			if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

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

			else if (checkTL() || CH == ' ' || CH == '\n' || CH == '\t') {
				put(TN);
				out(3, z);
				CS = H;
			}

			else {
				handleError(InvalidNumber, currentLine, currentPosition);
				CS = ER;
			}

			break;

		case C:

			while (CH != '}') {
				if (CH == '\0') {
					handleError(ExpectedEndCommentary);
					CS = ER;
					break;
				}
				else
					gc();
			}

			if (CH == '}') {
				nill();
				add();
				gc();
				CS = C1;
			}

			break;

		case C1:

			CS = H;
			break;

		case V:
			return true;

		case ER:
			return false;
		}
	}
}


