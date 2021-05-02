#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include "node.h"

extern std::ifstream infile;

Node * parser();
void print_tree(Node *);

#endif