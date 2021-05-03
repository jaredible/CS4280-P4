#include <iostream>
#include "parser.h"
#include "semantics.h"
#include "shared.h"
#include <stdio.h>
#include <string>

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
		std::string infile_name = filename + ".fs21";
		std::string outfile_name = filename + ".asm";

		/* Open input file */
		infile.open(infile_name);

		/* Cannot open input file */
		if (!infile) {
			std::cerr << "Error opening " << infile_name << " for reading" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Prepare output file for writing */
		outfile.open(outfile_name, std::ios::trunc);

		/* Cannot open output file */
		if (!outfile) {
			std::cerr << "Error opening " << outfile_name << " for writing" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Execute parser */
		Node * root = parser();
		if (PRINT_TREE) print_tree(root);
		generate(root);

		/* Close files */
		infile.close();
		outfile.close();
	}
	
	/* No filename argument given */
	else if (argc == 1) {
		/* Create temporary file to hold keyboard input */
		std::ofstream tempfile;
		std::string filename = "kb.txt";

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

		std::string outfile_name = "kb.asm";

		/* Prepare output file for writing */
		outfile.open(outfile_name, std::ios::trunc);

		/* Cannot open output file */
		if (!outfile) {
			std::cerr << "Error opening " << outfile_name << " for writing" << std::endl;
			exit(EXIT_FAILURE);
		}

		/* Execute parser */
		Node * root = parser();
		if (PRINT_TREE) print_tree(root);
		generate(root);

		/* Close files */
		infile.close();
		outfile.close();
	}
}