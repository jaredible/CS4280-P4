Storage = Local
Driver function is "void generate(Node *)" and implemented in "src/semantics.cpp"

Description:
Compiles an fs21 source file into an assembly (asm) binary in the same directory as the source

Testing:
Exist in the directory "test-files"

Installation:
make

Usage: (Note: Press Ctrl+D to finalize keyboard input)
bin/P4 < file
bin/P4 [filename]

Adjustments:
- Input files require blank last line
- Filenames implicitly have "fs21" extension
- Global vars are also stored on stack
- Assumes label(s) will always be declared when using respective goto(s)

Issues:
- Comments don't increment line count
- No static semantic checks for label identifiers
