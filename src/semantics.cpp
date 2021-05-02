#include <iomanip>
#include <iostream>
#include "semantics.h"
#include "shared.h"
#include "token.h"

/* Allow 100 vars on stack */
const int max_stack_size = 100;

static Token stack[max_stack_size];
static std::string temp_vars[max_stack_size];

static int var_count = 0;
static int temp_var_count = 0;
static int label_count = 0;
static int scope_begin = 0;
static bool global_scope = true;

static std::string get_temp_var();
static std::string get_label();
static void push(Token);
static void pop();
static int find(Token);
static int var_exists(Token);
static void print_stack();

void semantics_check(Node * node, int count) {
	/* Guard against invalid nodes */
	if (node == nullptr) return;

	/* Display node's name and variables currently on stack */
	if (DEBUG) {
		std::cout << std::left << std::setw(10) << node->name << " ";
		print_stack();
	}

	/* <program> non-terminal */
	if (node->name == "<program>") {
		unsigned int num_vars = 0;

		/* Check all node's children's semantics */
		for (Node * child : node->children)
			if (child != nullptr) semantics_check(child, num_vars);
		
		outfile << "STOP\n";
		for (auto i = 0; i < max_stack_size; i++)
			if (temp_vars[i] != "") outfile << temp_vars[i] << " 0\n";
	
	/* <vars> non-terminal */
	} else if (node->name == "<vars>") {
		int tos_distance = find(node->tokens[1]);
		scope_begin = var_count;

		/* Check if var has already been defined */
		if (tos_distance == -1 || tos_distance > count) {
			/* Push identifier onto stack */
			push(node->tokens[1]);
			count++;
		} else if (tos_distance < count) {
			std::cout << "ERROR: " << node->tokens[1].value << " has already been defined" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Traverse node's first child's semantics */
		if (node->children[0] != nullptr) semantics_check(node->children[0], count);

		/* Print non-global stack vars */
		if (!global_scope) print_stack();

	/* <block> non-terminal */
	} else if (node->name == "<block>") {
		unsigned int num_vars = 0;
		scope_begin = var_count;

		/* Print global stack vars */
		if (global_scope) print_stack();
		/* Not in global scope anymore */
		global_scope = false;

		/* Traverse all node's children's semantics */
		for (Node * child : node->children)
			if (child != nullptr) semantics_check(child, num_vars);

		/* Remove all identifiers in current scope */
		pop();
	
	/* <expr> non-terminal */
	} else if (node->name == "<expr>") {
		if (node->tokens.size() > 0 && node->tokens[0].id == TK_MINUS) {
			if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
			std::string temp_var = get_temp_var();
			outfile << "STORE " << temp_var << "\n";
			if (node->children.size() > 1 && node->children[1] != nullptr) semantics_check(node->children[1], count);
			outfile << "SUB " << temp_var << "\n";
		} else if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
	
	/* <N> non-terminal */
	} else if (node->name == "<N>") {
		if (node->tokens.size() > 0 && (node->tokens[0].id == TK_SLASH)) {
			if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
			std::string temp_var = get_temp_var();
			outfile << "STORE " << temp_var << "\n";
			if (node->children.size() > 1 && node->children[1] != nullptr) semantics_check(node->children[1], count);
			outfile << "DIV " << temp_var << "\n";
		} else if (node->tokens.size() > 0 && (node->tokens[0].id == TK_ASTERISK)) {
			if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
			std::string temp_var = get_temp_var();
			outfile << "STORE " << temp_var << "\n";
			if (node->children.size() > 1 && node->children[1] != nullptr) semantics_check(node->children[1], count);
			outfile << "MULT " << temp_var << "\n";
		} else if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
	
	/* <A> non-terminal */
	} else if (node->name == "<A>") {
		if (node->tokens.size() > 0 && node->tokens[0].id == TK_PLUS) {
			if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
			std::string temp_var = get_temp_var();
			outfile << "STORE " << temp_var << "\n";
			if (node->children.size() > 1 && node->children[1] != nullptr) semantics_check(node->children[1], count);
			outfile << "ADD " << temp_var << "\n";
		} else if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
	
	/* <M> non-terminal */
	} else if (node->name == "<M>") {
		if (node->tokens.size() > 0 && node->tokens[0].id == TK_ASTERISK) {
			if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
			outfile << "MULT -1\n";
		} else if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
	
	/* <R> non-terminal */
	} else if (node->name == "<R>") {
		if (node->tokens.size() > 0) {
			if (node->tokens[0].id == TK_ID) {
				int var_location = var_exists(node->tokens[0]);
				if (var_location == -1) {
					std::cout << "ERROR: " << node->tokens[0].value << " has not been declared" << std::endl;
					exit(EXIT_FAILURE);
				}
				outfile << "STACKR " << var_location << "\n";
			} else if (node->tokens[0].id == TK_INT) {
				outfile << "LOAD " << node->tokens[0].value << "\n";
			}
		} else if (node->children.size() > 0 && node->children[0] != nullptr) semantics_check(node->children[0], count);
	
	/* <in> non-terminal */
	} else if (node->name == "<in>") {
		int var_location = var_exists(node->tokens[1]);
		if (var_location == -1) {
			std::cout << "ERROR: " << node->tokens[1].value << " has not been declared" << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string temp_var = get_temp_var();
		outfile << "READ " << temp_var << "\n";
		outfile << "LOAD " << temp_var << "\n";
		outfile << "STACKW " << var_location << "\n";
	
	/* <out> non-terminal */
	} else if (node->name == "<out>") {
		if (node->children[0] != nullptr) semantics_check(node->children[0], count);
		std::string temp_var = get_temp_var();
		outfile << "STORE " << temp_var << "\n";
		outfile << "WRITE " << temp_var << "\n";
	
	/* <assign> non-terminal */
	} else if (node->name == "<assign>") {
		if (node->children[0] != nullptr) semantics_check(node->children[0], count);
		int var_location = var_exists(node->tokens[1]);
		if (var_location == -1) {
			std::cout << "ERROR: " << node->tokens[1].value << " has not been declared" << std::endl;
			exit(EXIT_FAILURE);
		}
		outfile << "STACKW " << var_location << "\n";
	
	/* any other non-terminals */
	} else {
		/* Traverse all node's children's semantics */
		for (Node * child : node->children)
			if (child != nullptr) semantics_check(child, count);
	}
}

static std::string get_temp_var() {
	std::string temp_var = "T" + std::to_string(temp_var_count + 1);
	temp_vars[temp_var_count] = temp_var;
	temp_var_count++;
	return temp_var;
}

static std::string get_label() {
	std::string label = "L" + std::to_string(label_count + 1);
	label_count++;
	return label;
}

/* Pushes given token onto stack, if possible */
static void push(Token tk) {
	/* Error if stack is full */
	if (var_count >= max_stack_size) {
		std::cout << "ERROR: Full stack" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Error if var has already been declared in current scope */
	for (int i = scope_begin; i < var_count; i++) {
		if (stack[i].value == tk.value) {
			std::cout << "ERROR: " << tk.value << " has already been declared" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	/* Append token to stack */
	stack[var_count++] = tk;
	outfile << "PUSH\n";
}

/* Pops current scope from stack */
static void pop() {
	/* Remove tokens on stack in current scope */
	for (auto i = var_count; i > scope_begin; i--) {
		var_count--;
		stack[i].value = "";
		outfile << "PUSH\n";
	}
}

/* Returns top-most index of token on stack */
static int find(Token tk) {
	/* From top of stack to scope beginning */
	for (int i = var_count; i >= scope_begin; i--) {
		/* Check token exists in scope */
		if (stack[i].value == tk.value) {
			/* Distance of current stack index from top */
			int tos = var_count - 1;
			return tos - i;
		}
	}
	/* Token not found */
	return -1;
}

/* Checks if a var exists anywhere on stack */
static int var_exists(Token tk) {
	/* From top of stack to bottom */
	for (int i = var_count - 1; i > -1; i--)
		/* Return var index */
		if (stack[i].value == tk.value) return var_count - 1 - i;
	/* Var not found */
	return -1;
}

/* Print stack vars inline space-separated */
static void print_stack() {
	/* For all possible tokens in stack */
	for (auto i = 0; i < max_stack_size; i++) {
		if (stack[i].value == "") {
			/* Only end the line if something on the stack has been printed */
			if (i > 0) std::cout << std::endl;
			break;
		}
		/* Print token's value */
		std::cout << stack[i].value << " ";
	}
}