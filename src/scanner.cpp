#include <iostream>
#include <map>
#include "scanner.h"
#include "token.h"

int FSA[23][23] = {
	/*    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21  22 */
	/*   ws   lc    d    =    <    >    :    +    -    *    /    %    .    (    )    ,    {    }    ;    [    ]  eof  uc */
	{     0,   1,   2,   3,  -2,  -2,  21,   8,   9,  10,  11,  12,  13,  14,  15,  20,  16,  17,  22,  18,  19,  -1,  -2 }, /* 0 */
	{   100,   1,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,   1 }, /* 1  TK_ID */
	{   101, 101,   2, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101 }, /* 2  TK_INT */
	{   102, 102, 102,   6,   5,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102 }, /* 3  TK_EQUAL */
	{   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103 }, /* 4  TK_GREATER_THAN_EQUAL */
	{   104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104 }, /* 5  TK_LESS_THAN_EQUAL */
	{   105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105 }, /* 6  TK_DOUBLE_EQUAL */
	{   106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106 }, /* 7  TK_DEFINE */
	{   107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107 }, /* 8  TK_PLUS */
	{   108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108 }, /* 9  TK_MINUS */
	{   109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109 }, /* 10 TK_ASTERISK */
	{   110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110 }, /* 11 TK_SLASH */
	{   111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111 }, /* 12 TK_PERCENT */
	{   112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112 }, /* 13 TK_PERIOD */
	{   113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113 }, /* 14 TK_LEFT_PAREN */
	{   114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114 }, /* 15 TK_RIGHT_PAREN */
	{   115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115 }, /* 16 TK_LEFT_BRACE */
	{   116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116 }, /* 17 TK_RIGHT_BRACE */
	{   117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117 }, /* 18 TK_LEFT_BRACKET */
	{   118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118 }, /* 19 TK_RIGHT_BRACKET */
	{   119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119 }, /* 20 TK_COMMA */
	{   120, 120, 120,   7, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120 }, /* 21 TK_COLON */
	{   121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121 }, /* 22 TK_SEMICOLON */
};

std::map<int, TokenType> finalStates = {
	/* Operators */
	{ 102, TK_EQUAL },
	{ 103, TK_GREATER_THAN_EQUAL },
	{ 104, TK_LESS_THAN_EQUAL },
	{ 105, TK_DOUBLE_EQUAL },
	{ 106, TK_DEFINE },
	{ 107, TK_PLUS },
	{ 108, TK_MINUS },
	{ 109, TK_ASTERISK },
	{ 110, TK_SLASH },
	{ 111, TK_PERCENT },
	{ 112, TK_PERIOD },

	/* Delimiters */
	{ 113, TK_LEFT_PAREN },
	{ 114, TK_RIGHT_PAREN },
	{ 115, TK_LEFT_BRACE },
	{ 116, TK_RIGHT_BRACE },
	{ 117, TK_LEFT_BRACKET },
	{ 118, TK_RIGHT_BRACKET },
	{ 119, TK_COMMA },
	{ 120, TK_COLON },
	{ 121, TK_SEMICOLON },

	/* Identifiers */
	{ 100, TK_ID },

	/* Integers */
	{ 101, TK_INT },

	/* EOF */
	{ -1, TK_EOF },
};

/* Maps keyword tokens to their corresponding strings */
std::map<std::string, TokenType> keywords = {
	{ "begin", TK_BEGIN },
	{ "end", TK_END },
	{ "loop", TK_LOOP },
	{ "whole", TK_WHOLE },
	{ "void", TK_VOID },
	{ "exit", TK_EXIT },
	{ "getter", TK_GETTER },
	{ "outter", TK_OUTTER },
	{ "main", TK_MAIN },
	{ "if", TK_IF },
	{ "then", TK_THEN },
	{ "assign", TK_ASSIGN },
	{ "data", TK_DATA },
	{ "proc", TK_PROC },
};

/* Maps allowed symbols to their corresponding column */
std::map<char, int> allowedSymbols = {
	/* Operators */
	{ '=', 3 },
	{ '<', 4 },
	{ '>', 5 },
	{ ':', 6 },
	{ '+', 7 },
	{ '-', 8 },
	{ '*', 9 },
	{ '/', 10 },
	{ '%', 11 },
	{ '.', 12 },

	/* Delimiters */
	{ '(', 13 },
	{ ')', 14 },
	{ ',', 15 },
	{ '{', 16 },
	{ '}', 17 },
	{ ';', 18 },
	{ '[', 19 },
	{ ']', 20 },
};

/* Given an input stream, returns tokens */
Token scan(std::ifstream& infile, unsigned int& line) {
	int currState = 0;
	int nextState = 0;
	char currChar = ' ';
	std::string currWord = "";

	/* Loop until a final state */
	while (currState < 100 && currState > -1) {
		infile.get(currChar);

		/* Start of start comment delimiter */
		if (currChar == '$') {
			infile.get(currChar);

			/* End of start comment delimiter */
			if (currChar == '$') {
				infile.get(currChar);

				/* Ignore characters in comment */
				while (true) {
					infile.get(currChar);

					/* Reached EOF */
					if (infile.eof()) {
						std::cerr << "Scanning Error: Invalid comment at line: " << line << std::endl;
						return Token(TK_ERROR, "Invalid comment", line);
					}

					/* Start of end comment delimiter */
					if (currChar == '$') {
						infile.get(currChar);

						/* End of end comment delimiter */
						if (currChar == '$') {
							infile.get(currChar);

							/* Exit comment */
							break;
						}
					}
				}
			}
		}

		/* FSA column index */
		int column = getColumn(currChar);

		/* Reached EOF */
		if (infile.eof()) column = 21;

		/* Invalid character */
		if (column == -2) {
			std::cerr << "Scanning Error: Invalid character \'" << currChar << "\' at line: " << line << std::endl;
			return Token(TK_ERROR, "Invalid char", line);
		}

		/* Set next state */
		nextState = FSA[currState][column];

		/* If next is final, then return respective token */
		if (nextState >= 100 || nextState == -1 || nextState == -2) {
			/* EOF state */
			if (nextState == -1) return Token(TK_EOF, "EOF", line);

			/* Invalid character */
			if (nextState == -2) {
				std::cerr << "Scanning Error: Invalid character \'" << currChar << "\' at line: " << line << std::endl;
				return Token(TK_ERROR, "Invalid ID", line);
			}

			/* Other states */
			infile.unget();
			return getToken(nextState, currWord, line);
		} else {
			/* Append non-whitespace characters to word */
			if (!isspace(currChar)) currWord += currChar;

			/* Invalid word length */
			if (currWord.length() > 8) {
				std::cerr << "Scanning Error: Invalid length of \'" << currWord << "\' at line: " << line << std::endl;
				return Token(TK_ERROR, "Invalid Length", line);
			}

			/* Current character is newline */
			if (currChar == '\n') line++;

			/* Go to next state */
			currState = nextState;
		}
	}

	/* Scanner failure */
	return Token(TK_ERROR, "Scanner Failure", line);
}

/* Returns FSA table column index */
int getColumn(char ch) {
	/* If letter or underscore */
	if (isalpha(ch) || ch == '_') {
		/* If uppercase letter */
		if (isupper(ch)) return 22;
		/* If lowercase letter */
		return 1;
	}
	/* If digit */
	if (isdigit(ch)) return 2;
	/* Is whitespace */
	if (isspace(ch)) return 0;
	/* Is EOF */
	if (ch == EOF) return 21;
	/* Check if allowed symbol */
	if (allowedSymbols.find(ch) != allowedSymbols.end()) return allowedSymbols[ch];
	/* Character not allowed */
	return -2;
}

/* Given a state, returns a token */
Token getToken(int state, std::string word, unsigned int line) {
	/* Default token */
	Token token = Token();

	/* Set token */
	if (finalStates.find(state) != finalStates.end()) {
		token.id = finalStates[state];
		token.value = word;
		token.line = line;
	}

	/* If keyword, modify token with respective keyword */
	if (keywords.find(word) != keywords.end()) token.id = keywords[word];

	return token;
}