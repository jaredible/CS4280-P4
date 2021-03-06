#include <iostream>
#include "parser.h"
#include "semantics.h"
#include "shared.h"
#include <stdio.h>
#include <string>

const std::string INFILE_EXTENSION = "ss21";
const std::string OUTFILE_EXTENSION = "asm";
const std::string KEYBOARD_TEMPFILE_NAME = "kb";
const std::string KEYBOARD_TEMPFILE_EXTENSION = "txt";

std::ofstream outfile;

int main(int argc, char ** argv) {
	/* Too many arguments */
	if (argc > 2) {
		std::cerr << "Too many arguments given" << std::endl;
		exit(EXIT_FAILURE);
	}

	/* Filename argument given */
	else if (argc == 2) {
		/* Get filename and append extension */
		std::string filename = argv[1];
		std::string infile_name = filename + "." + INFILE_EXTENSION;
		std::string outfile_name = filename + "." + OUTFILE_EXTENSION;

		/* Open input file */
		infile.open(infile_name);

		/* Cannot open input file */
		if (!infile) {
			std::cerr << "Error opening " << infile_name << " for reading" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Execute parser */
		Node * root = parser();
		if (PRINT_TREE) print_tree(root);

		/* Prepare output file for writing */
		outfile.open(outfile_name, std::ios::trunc);

		/* Cannot open output file */
		if (!outfile) {
			std::cerr << "Error opening " << outfile_name << " for writing" << std::endl;
			exit(EXIT_FAILURE);
		}

		generate(root);
		std::cout << "Generated target file '" << outfile_name << "'" << std::endl;

		/* Close files */
		infile.close();
		outfile.close();
	}
	
	/* No filename argument given */
	else if (argc == 1) {
		/* Create temporary file to hold keyboard input */
		std::ofstream tempfile;
		std::string filename = KEYBOARD_TEMPFILE_NAME + "." + KEYBOARD_TEMPFILE_EXTENSION;

		tempfile.open(filename, std::ios::trunc);

		/* Cannot open tempory file */
		if (!outfile) {
			std::cerr << "Error opening " << filename << " for writing" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Helper character */
		char ch;

		/* Write keyboard input into temporary file */
		while (std::cin >> std::noskipws >> ch)
			tempfile << ch;

		/* Close temporary file */
		tempfile.close();

		/* Open input file */
		infile.open(filename);

		/* Cannot open input file */
		if (!infile) {
			std::cerr << "Error opening " << filename << " for reading" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string outfile_name = KEYBOARD_TEMPFILE_NAME + "." + OUTFILE_EXTENSION;

		/* Execute parser */
		Node * root = parser();
		if (PRINT_TREE) print_tree(root);

		/* Prepare output file for writing */
		outfile.open(outfile_name, std::ios::trunc);

		/* Cannot open output file */
		if (!outfile) {
			std::cerr << "Error opening " << outfile_name << " for writing" << std::endl;
			exit(EXIT_FAILURE);
		}

		generate(root);
		std::cout << "Generated target file '" << outfile_name << "'" << std::endl;

		/* Close files */
		infile.close();
		outfile.close();
	}
}