#ifndef NODE_H
#define NODE_H

#include <string>
#include "token.h"
#include <vector>

struct Node {
	std::string name;
	int depth;

	std::vector<Token> tokens;
	std::vector<Node *> children;

	Node() {
		this->name = "";
		this->depth = 0;
	}

	Node(std::string name, int depth) {
		this->name = name;
		this->depth = depth;
	}
};

#endif