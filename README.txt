Storage = Local
Driver function is "void generate(Node *)" and implemented in "src/semantics.cpp"

Description:
Compiles an fs21 program into an ASM file in the same directory as the source file

Build:
make

Program:
Located in directory "bin"

Usage: (Note: Ctrl+D to finalize keyboard input)
./compfs < file
./compfs [filename]

Tests:
Located in directory "test-files"

Adjustments:
- Input files require blank last line
- Filenames implicitly have "fs21" extension
- Global vars are also stored on stack
- Assumes label(s) will always be declared when using respective goto(s)

Issues:
- Comments don't increment line count
- No static semantic checks for label identifiers

BNF:
<program> -> <vars> main <block>
<block> -> begin <vars> <stats> end
<vars> -> empty | data Identifier := Integer ; <vars>
<expr> -> <N> - <expr> | <N>
<N> -> <A> / <N> | <A> * <N> | <A>
<A> -> <M> + <A> | <M>
<M> -> * <M> | <R>
<R> -> ( <expr> ) | Identifier | Integer
<stats> -> <stat> <mStat>
<mStat> -> empty | <stat> <mStat>
<stat> -> <in> ; | <out> ; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
<in> -> getter Identifier
<out> -> outter <expr>
<if> -> if [ <expr> <RO> <expr> ] then <stat>
<loop> -> loop [ <expr> <RO> <expr> ] <stat>
<assign> -> assign Identifier := <expr>
<RO> -> => | =< | == | [ == ] (three tokens) | %
<label> -> void Identifier
<goto> -> proc Identifier
