Storage = Local
Driver function is "void generate(Node *)" and implemented in "src/semantics.cpp"

Output:
- All tokens on stack are displayed for every new scope declaring variables

Tests:
 - Test-files exists in the directory "test-files"
 - There are several good and bad test-files

Installation:
make

Usage: (Note: Press Ctrl+D to finalize keyboard input)
bin/P4 < file
bin/P4 [filename]

Adjustments:
- Input files require blank last line
- Filenames implicitly have "fs21" extension

Issues:
- Comments don't increment line count
- No semantic checking for label identifiers
