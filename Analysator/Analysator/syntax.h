#pragma once
#include "lexeme.h"

extern std::string LEX;

bool syntaxScan();
void gl();
bool EQ(std::string S);
bool ID();
bool NUM();
void PROGRAMM();
void DESC();
void TYPE();
void OPERATOR();
void COMOPERATOR();
void ASSIGN();
void IF();
void FOR();
void WHILE();
void ENTER();
void DISPLAY();
void EXPRESSION();
void OPERAND();
void TERM();
void FACTOR();