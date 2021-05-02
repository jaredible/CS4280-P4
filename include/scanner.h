#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <string>
#include "token.h"

Token scan(std::ifstream&, unsigned int&);
int getColumn(char);
Token getToken(int, std::string, unsigned int);

#endif