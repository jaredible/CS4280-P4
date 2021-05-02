#include <iomanip>
#include <iostream>
#include "parser.h"
#include "scanner.h"
#include "shared.h"
#include "token.h"
#include <vector>

std::ifstream infile;
unsigned int line = 1;
Token tk;

std::string tokens[] = {
	/* Keyword Tokens */
	"TK_BEGIN",
	"TK_END",
	"TK_LOOP",
	"TK_WHOLE",
	"TK_VOID",
	"TK_EXIT",
	"TK_GETTER",
	"TK_OUTTER",
	"TK_MAIN",
	"TK_IF",
	"TK_THEN",
	"TK_ASSIGN",
	"TK_DATA",
	"TK_PROC",

	/* Operator Tokens */
	"TK_EQUAL",
	"TK_GREATER_THAN_EQUAL",
	"TK_LESS_THAN_EQUAL",
	"TK_DOUBLE_EQUAL",
	"TK_DEFINE",
	"TK_PLUS",
	"TK_MINUS",
	"TK_ASTERISK",
	"TK_SLASH",
	"TK_PERCENT",
	"TK_PERIOD",

	/* Delimiter Tokens */
	"TK_LEFT_PAREN",
	"TK_RIGHT_PAREN",
	"TK_LEFT_BRACE",
	"TK_RIGHT_BRACE",
	"TK_LEFT_BRACKET",
	"TK_RIGHT_BRACKET",
	"TK_COMMA",
	"TK_COLON",
	"TK_SEMICOLON",

	/* Identifier Tokens */
	"TK_ID",

	/* Integer Tokens */
	"TK_INT",

	/* EOF Tokens */
	"TK_EOF",

	/* Error Tokens */
	"TK_ERROR"
};

/* Non-terminal functions */
static Node * parser_program();
static Node * parser_block(int);
static Node * parser_vars(int);
static Node * parser_expr(int);
static Node * parser_n(int);
static Node * parser_a(int);
static Node * parser_m(int);
static Node * parser_r(int);
static Node * parser_stats(int);
static Node * parser_mstat(int);
static Node * parser_stat(int);
static Node * parser_in(int);
static Node * parser_out(int);
static Node * parser_if(int);
static Node * parser_loop(int);
static Node * parser_assign(int);
static Node * parser_ro(int);
static Node * parser_label(int);
static Node * parser_goto(int);

/* Utility functions */
static void consume(Node *);
static void error(TokenType, Token);
static void error_stat(Token);
static bool is_stat(TokenType);
static bool is_ro(TokenType);
static void print_token(Token);

/* Auxilary function */
Node * parser() {
	Node * node = nullptr;

	/* Consume first token */
	consume(node);

	/* Begin parser with starting non-terminal */
	node = parser_program();

	if (tk.id == TK_EOF) {
		if (DEBUG) std::cout << "Parse Success" << std::endl;
		return node;
	} else error(TK_EOF, tk);

	return nullptr;
}

/* Prints parse tree */
void print_tree(Node * node) {
	/* Empty tree guard */
	if (node == nullptr) return;

	/* Print non-terminal */
	std::string indents((node->depth * 2), ' ');
	std::cout << indents;
	std::cout << node->name;

	/* Print tokens in non-terminal */
	for (Token token : node->tokens)
		std::cout << " " << token.value;
	/* Print identifier token line number */
	if (node->tokens.size() > 1 && node->tokens[1].id == TK_ID)
		std::cout << " On Line: " << node->tokens[1].line; 
	std::cout << std::endl;

	/* Print tree of all node's child nodes */
	for (Node * child : node->children)
		if (child != nullptr) print_tree(child);
}

/* <program> -> <vars> main <block> */
static Node * parser_program() {
	int depth = 0;
	Node * node = new Node("<program>", depth);

	node->children.push_back(parser_vars(depth));

	if (tk.id == TK_MAIN) {
		consume(node);

		node->children.push_back(parser_block(depth));

		return node;
	} else error(TK_MAIN, tk);

	return nullptr;
}

/* <block> -> begin <vars> <stats> end */
static Node * parser_block(int depth) {
	depth++;
	Node * node = new Node("<block>", depth);

	if (tk.id == TK_BEGIN) {
		consume(node);

		node->children.push_back(parser_vars(depth));
		node->children.push_back(parser_stats(depth));

		if (tk.id == TK_END) {
			consume(node);

			return node;
		} else error(TK_END, tk);
	} else error(TK_BEGIN, tk);

	return nullptr;
}

/* <vars> -> empty | data Identifer := Integer ; <vars> */
static Node * parser_vars(int depth) {
	depth++;
	Node * node = new Node("<vars>", depth);

	if (tk.id == TK_DATA) {
		consume(node);

		if (tk.id == TK_ID) {
			consume(node);

			if (tk.id == TK_DEFINE) {
				consume(node);

				if (tk.id == TK_INT) {
					consume(node);

					if (tk.id == TK_SEMICOLON) {
						consume(node);

						node->children.push_back(parser_vars(depth));

						return node;
					} else error(TK_SEMICOLON, tk);
				} else error(TK_INT, tk);
			} else error(TK_DEFINE, tk);
		} else error(TK_ID, tk);
	}

	return nullptr;
}

/* <expr> -> <N> - <expr> | <N> */
static Node * parser_expr(int depth) {
	depth++;
	Node * node = new Node("<expr>", depth);

	node->children.push_back(parser_n(depth));

	if (tk.id == TK_MINUS) {
		consume(node);

		node->children.push_back(parser_expr(depth));
		
		return node;
	} else return node;
}

/* <N> -> <A> / <N> | <A> * <N> | <A> */
static Node * parser_n(int depth) {
	depth++;
	Node * node = new Node("<N>", depth);

	node->children.push_back(parser_a(depth));

	if (tk.id == TK_SLASH) {
		consume(node);

		node->children.push_back(parser_n(depth));

		return node;
	} else if (tk.id == TK_ASTERISK) {
		consume(node);

		node->children.push_back(parser_n(depth));

		return node;
	} else return node;
}

/* <A> -> <M> + <A> | <M> */
static Node * parser_a(int depth) {
	depth++;
	Node * node = new Node("<A>", depth);

	node->children.push_back(parser_m(depth));

	if (tk.id == TK_PLUS) {
		consume(node);

		node->children.push_back(parser_a(depth));

		return node;
	} else return node;
}

/* <M> -> * <M> | <R> */
static Node * parser_m(int depth) {
	depth++;
	Node * node = new Node("<M>", depth);

	if (tk.id == TK_ASTERISK) {
		consume(node);

		node->children.push_back(parser_m(depth));

		return node;
	} else {
		node->children.push_back(parser_r(depth));

		return node;
	}
}

/* <R> -> ( <expr> ) | Identifier | Integer */
static Node * parser_r(int depth) {
	depth++;
	Node * node = new Node("<R>", depth);

	if (tk.id == TK_LEFT_PAREN) {
		consume(node);

		node->children.push_back(parser_expr(depth));

		if (tk.id == TK_RIGHT_PAREN) {
			consume(node);

			return node;
		} else error(TK_RIGHT_PAREN, tk);
	} else if (tk.id == TK_ID) {
		consume(node);

		return node;
	} else if (tk.id == TK_INT) {
		consume(node);

		return node;
	} else error(TK_LEFT_PAREN, tk);

	return nullptr;
}

/* <stats> -> <stat> <mStat> */
static Node * parser_stats(int depth) {
	depth++;
	Node * node = new Node("<stats>", depth);

	node->children.push_back(parser_stat(depth));
	node->children.push_back(parser_mstat(depth));

	return node;
}

/* <mStat> -> empty | <stat> <mStat> */
static Node * parser_mstat(int depth) {
	depth++;
	Node * node = new Node("<mStat>", depth);

	if (is_stat(tk.id)) {
		node->children.push_back(parser_stat(depth));
		node->children.push_back(parser_mstat(depth));

		return node;
	} else return nullptr;
}

/* <stat> -> <in> ; | <out> ; | <block> ; | <if> ; | <loop> ; | <assign> ; | <goto> ; | <labal> ; */
static Node * parser_stat(int depth) {
	depth++;
	Node * node = new Node("<stat>", depth);

	if (tk.id == TK_GETTER) {
		node->children.push_back(parser_in(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_OUTTER) {
		node->children.push_back(parser_out(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_BEGIN) {
		node->children.push_back(parser_block(depth));

		return node;
	} else if (tk.id == TK_IF) {
		node->children.push_back(parser_if(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_LOOP) {
		node->children.push_back(parser_loop(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_ASSIGN) {
		node->children.push_back(parser_assign(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_PROC) {
		node->children.push_back(parser_goto(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else if (tk.id == TK_VOID) {
		node->children.push_back(parser_label(depth));

		if (tk.id == TK_SEMICOLON) {
			consume(node);

			return node;
		} else error(TK_SEMICOLON, tk);
	} else error_stat(tk);

	return nullptr;
}

/* <in> -> getter Identifier */
static Node * parser_in(int depth) {
	depth++;
	Node * node = new Node("<in>", depth);

	if (tk.id == TK_GETTER) {
		consume(node);

		if (tk.id == TK_ID) {
			consume(node);

			return node;
		} else error(TK_ID, tk);
	} else error(TK_GETTER, tk);

	return nullptr;
}

/* <out> -> outter <expr> */
static Node * parser_out(int depth) {
	depth++;
	Node * node = new Node("<out>", depth);

	if (tk.id == TK_OUTTER) {
		consume(node);

		node->children.push_back(parser_expr(depth));

		return node;
	} else error(TK_OUTTER, tk);

	return nullptr;
}

/* <if> -> if [ <expr> <RO> <expr> ] then <stat> */
static Node * parser_if(int depth) {
	depth++;
	Node * node = new Node("<if>", depth);

	if (tk.id == TK_IF) {
		consume(node);

		if (tk.id == TK_LEFT_BRACKET) {
			consume(node);

			node->children.push_back(parser_expr(depth));
			node->children.push_back(parser_ro(depth));
			node->children.push_back(parser_expr(depth));

			if (tk.id == TK_RIGHT_BRACKET) {
				consume(node);

				if (tk.id == TK_THEN) {
					consume(node);

					node->children.push_back(parser_stat(depth));

					return node;
				} else error(TK_THEN, tk);
			} else error(TK_RIGHT_BRACKET, tk);
		} else error(TK_LEFT_BRACKET, tk);
	} else error(TK_IF, tk);

	return nullptr;
}

/* <loop> -> loop [ <expr> <RO> <expr> ] <stat> */
static Node * parser_loop(int depth) {
	depth++;
	Node * node = new Node("<loop>", depth);
	
	if (tk.id == TK_LOOP) {
		consume(node);

		if (tk.id == TK_LEFT_BRACKET) {
			consume(node);

			node->children.push_back(parser_expr(depth));
			node->children.push_back(parser_ro(depth));
			node->children.push_back(parser_expr(depth));

			if (tk.id == TK_RIGHT_BRACKET) {
				consume(node);

				node->children.push_back(parser_stat(depth));

				return node;
			} else error(TK_RIGHT_BRACKET, tk);
		} else error(TK_LEFT_BRACKET, tk);
	} else error(TK_IF, tk);

	return nullptr;
}

/* <assign> -> assign Identifier := <expr> */
static Node * parser_assign(int depth) {
	depth++;
	Node * node = new Node("<assign>", depth);

	if (tk.id == TK_ASSIGN) {
		consume(node);

		if (tk.id == TK_ID) {
			consume(node);

			if (tk.id == TK_DEFINE) {
				consume(node);

				node->children.push_back(parser_expr(depth));

				return node;
			} else error(TK_DEFINE, tk);
		} else error(TK_ID, tk);
	} else error(TK_ASSIGN, tk);

	return nullptr;
}

/* <RO> -> => | =< | == | [==] | % */
static Node * parser_ro(int depth) {
	depth++;
	Node * node = new Node("<RO>", depth);

	if (is_ro(tk.id)) {
		consume(node);

		if (node->tokens[node->tokens.size() - 1].id == TK_LEFT_BRACKET) {
			if (tk.id == TK_DOUBLE_EQUAL) {
				consume(node);

				if (tk.id == TK_RIGHT_BRACKET) {
					consume(node);

					return node;
				} else error(TK_RIGHT_BRACKET, tk);
			} else error(TK_DOUBLE_EQUAL, tk);
		}

		return node;
	}

	return nullptr;
}

/* <label> -> void Identifier */
static Node * parser_label(int depth) {
	depth++;
	Node * node = new Node("<label>", depth);

	if (tk.id == TK_VOID) {
		consume(node);

		if (tk.id == TK_ID) {
			consume(node);

			return node;
		} else error(TK_ID, tk);
	} else error(TK_VOID, tk);

	return nullptr;
}

/* <goto> -> proc Identifier */
static Node * parser_goto(int depth) {
	depth++;
	Node * node = new Node("<goto>", depth);

	if (tk.id == TK_PROC) {
		consume(node);

		if (tk.id == TK_ID) {
			consume(node);

			return node;
		} else error(TK_ID, tk);
	} else error(TK_PROC, tk);

	return nullptr;
}

static void consume(Node * node) {
	/* Append token to node */
	if (node != nullptr) node->tokens.push_back(tk);

	/* Scan for next token */
	tk = scan(infile, line);
	
	/* Print current token */
	print_token(tk);
}

/* default error function */
static void error(TokenType expected, Token received) {
	std::cout << "Parse Error" << std::endl;
	std::cout << "Expected token: " << tokens[expected] << std::endl;
	std::cout << "Received token: " << tokens[received.id];
	std::cout << " At line: " << received.line << std::endl;
	exit(EXIT_FAILURE);
}

/* <stat> error function */
static void error_stat(Token recieved) {
	std::cout << "Parse Error" << std::endl;
	std::cout << "Expected tokens: " << tokens[TK_GETTER];
	std::cout << ", " << tokens[TK_OUTTER];
	std::cout << ", " << tokens[TK_BEGIN];
	std::cout << ", " << tokens[TK_IF]; 
	std::cout << ", " << tokens[TK_LOOP];
	std::cout << ", " << tokens[TK_ASSIGN];
	std::cout << ", " << tokens[TK_PROC];
	std::cout << ", or " << tokens[TK_VOID] << std::endl;
	std::cout << "Recieved token: " << tokens[recieved.id];
	std::cout << " At line: " << recieved.line << std::endl;
	exit(EXIT_FAILURE);
}

static bool is_stat(TokenType tk) {
	switch (tk) {
		case TK_GETTER:
		case TK_OUTTER:
		case TK_BEGIN:
		case TK_IF:
		case TK_LOOP:
		case TK_ASSIGN:
		case TK_PROC:
		case TK_VOID:
			return true;
		default:
			return false;
	}
}

static bool is_ro(TokenType tk) {
	switch (tk) {
		case TK_LESS_THAN_EQUAL:
		case TK_GREATER_THAN_EQUAL:
		case TK_DOUBLE_EQUAL:
		case TK_LEFT_BRACKET:
		case TK_PERCENT:
			return true;
		default:
			return false;
	}
}

static void print_token(Token token) {
	if (!DEBUG) return;
	std::cout << std::left;
	std::cout << "Line: " << std::setw(5) << token.line;
	std::cout << "\tToken: " << std::setw(20) << tokens[token.id];
	std::cout << "\tValue: " << std::setw(10) << token.value;
	std::cout << std::endl;
}