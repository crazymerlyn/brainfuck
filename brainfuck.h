#ifndef INCLUDE_BRAINFUCK_H
#define INCLUDE_BRAINFUCK_H


#define BF_TOKEN_LOOP_START     '['
#define BF_TOKEN_LOOP_END       ']'
#define BF_TOKEN_DATA_INCREMENT '+'
#define BF_TOKEN_DATA_DECREMENT '-'
#define BF_TOKEN_PTR_INCREMENT  '>'
#define BF_TOKEN_PTR_DECREMENT  '<'
#define BF_TOKEN_OUTPUT         '.'
#define BF_TOKEN_INPUT          ','

typedef enum {
    BF_DATA,
    BF_PTR,
    BF_OUTPUT,
    BF_INPUT,
    BF_LOOP,
    BF_END
} InstructionType;

struct _Instruction {
    InstructionType type;
    int value;
    struct _Instruction* loop;
};

typedef struct _Instruction Instruction;

typedef struct {
    unsigned char* tape;
    size_t tape_size;
    size_t tape_index;
} Context;

size_t closing_bracket_pos(char* str, size_t start);
Instruction* parseString(char* str);
Instruction* parseSubstring(char* str, size_t start, size_t end);

Context getContext();
void executeInstructions(Instruction* instructions, Context* context);

void interpret(char* str);

void freeInstructions(Instruction*);

#endif /* INCLUDE_BRAINFUCK_H */

