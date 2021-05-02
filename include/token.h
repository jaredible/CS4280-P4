#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
	/* Keyword Tokens */
	TK_BEGIN,	/* begin */
	TK_END,		/* end */
	TK_LOOP,	/* loop */
	TK_WHOLE,	/* whole */
	TK_VOID,	/* void */
	TK_EXIT,	/* exit */
	TK_GETTER,	/* getter */
	TK_OUTTER,	/* outter */
	TK_MAIN,	/* main */
	TK_IF,		/* if */
	TK_THEN,	/* then */
	TK_ASSIGN,	/* assign */
	TK_DATA,	/* data */
	TK_PROC,	/* goto */

	/* Operator Tokens */
	TK_EQUAL,
	TK_GREATER_THAN_EQUAL,
	TK_LESS_THAN_EQUAL,
	TK_DOUBLE_EQUAL,
	TK_DEFINE,
	TK_PLUS,
	TK_MINUS,
	TK_ASTERISK,
	TK_SLASH,
	TK_PERCENT,
	TK_PERIOD,

	/* Delimiter Tokens */
	TK_LEFT_PAREN,
	TK_RIGHT_PAREN,
	TK_LEFT_BRACE,
	TK_RIGHT_BRACE,
	TK_LEFT_BRACKET,
	TK_RIGHT_BRACKET,
	TK_COMMA,
	TK_COLON,
	TK_SEMICOLON,

	/* Identifier Tokens */
	TK_ID,

	/* Integer Tokens */
	TK_INT,

	/* EOF Tokens */
	TK_EOF,

	/* Error Tokens */
	TK_ERROR
};

struct Token {
	TokenType id;
	std::string value;
	unsigned int line;

	/* Default token */
	Token() {
		this->id = TK_ERROR;
		this->value = "";
		this->line = 0;
	}

	/* Standard token */
	Token(TokenType id, std::string value, unsigned int line) {
		this->id = id;
		this->value = value;
		this->line = line;
	}
};

#endif