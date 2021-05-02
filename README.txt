Storage = Local
Driver function is "void semantics_check(Node *, int)" and implemented in "src/semantics.cpp"

Output:
- All tokens on stack are displayed for every new scope declaring variables

Tests:
 - Test-files exists in the directory "test-files"
 - There are several good and bad test-files

Installation:
make

Usage: (Note: Press Ctrl+D to finalize keyboard input)
bin/P3 < file
bin/P3 [filename]

Adjustments:
- Input files require blank last line
- Filenames implicitly have "fs21" extension

Issues:
- Comments don't increment line count
