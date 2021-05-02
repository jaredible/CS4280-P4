#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <fstream>
#include "node.h"

extern std::ofstream outfile;

void semantics_check(Node *, int);

#endif