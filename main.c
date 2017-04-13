#include <stdio.h>
#include "brainfuck.h"

int main(int argc, char** argv)
{
    char* program = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

    if (argc > 1) {
        program = argv[1];
    }

    interpret(program);

    return 0;
}
