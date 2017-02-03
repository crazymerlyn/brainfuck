#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "brainfuck.h"

size_t closing_bracket_pos(char* str, size_t start)
{
    int counter = 0;
    for (int i = start; str[i]; ++i) {
        if (str[i] == BF_TOKEN_LOOP_START) counter++;
        else if (str[i] == BF_TOKEN_LOOP_END) {
            counter--;
            if (counter == -1) return i;
        }
    }
    return -1;
}

Instruction* parseString(char* str)
{
    return parseSubstring(str, 0, strlen(str));
}

#define BUF_SIZE 64
Instruction* parseSubstring(char* str, size_t start, size_t end)
{
    size_t buf_size = BUF_SIZE;
    size_t index = 0, i = start;
    Instruction* instructions = malloc(sizeof(Instruction) * buf_size);
    if (!instructions) {
        fprintf(stderr, "bfi: allocation error.\n");
        exit(EXIT_FAILURE);
    }

    while(i < end) {
        bool instruction_added = true;
        switch(str[i]) {
        case BF_TOKEN_DATA_INCREMENT:
        case BF_TOKEN_DATA_DECREMENT:
            instructions[index].type = BF_DATA;
            instructions[index].value = 0;
            while(str[i] == BF_TOKEN_DATA_DECREMENT 
                    || str[i] == BF_TOKEN_DATA_INCREMENT) {
                if (str[i] == BF_TOKEN_DATA_INCREMENT) instructions[index].value += 1;
                else if (str[i] == BF_TOKEN_DATA_DECREMENT) instructions[index].value -= 1;
                i++;
            }
            break;

        case BF_TOKEN_PTR_INCREMENT:
        case BF_TOKEN_PTR_DECREMENT:
            instructions[index].type = BF_PTR;
            instructions[index].value = 0;
            while(str[i] == BF_TOKEN_PTR_DECREMENT 
                    || str[i] == BF_TOKEN_PTR_INCREMENT) {
                if (str[i] == BF_TOKEN_PTR_INCREMENT) instructions[index].value += 1;
                else if (str[i] == BF_TOKEN_PTR_DECREMENT) instructions[index].value -= 1;
                i++;
            }
            break;

        case BF_TOKEN_LOOP_START:
            instructions[index].type = BF_LOOP;
            instructions[index].loop = parseSubstring(str, i+1, closing_bracket_pos(str, i+1));
            i = closing_bracket_pos(str, i+1) + 1;
            break;

        case BF_TOKEN_OUTPUT:
            instructions[index].type = BF_OUTPUT;
            instructions[index].value = 1;
            while(str[++i] == BF_OUTPUT) {
                instructions[index].value += 1;
            }
            break;

        case BF_TOKEN_INPUT:
            instructions[index].type = BF_INPUT;
            instructions[index].value = 1;
            while (str[++i] == BF_TOKEN_INPUT) {
                instructions[index].value += 1;
            }
            break;

        default:
            ++i;
            instruction_added = false;
            break;
        }
        if (instruction_added) {
            index += 1;
            if (index >= buf_size) {
                buf_size += BUF_SIZE;
                instructions = realloc(instructions, sizeof(Instruction) * buf_size);

                if (!instructions) {
                    fprintf(stderr, "bfi: allocation error.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    instructions[index].type = BF_END;
    return instructions;
}

#define TAPE_SIZE 30000
Context getContext()
{
    Context context;
    context.tape = (unsigned char*) calloc(TAPE_SIZE, sizeof(char));
    context.tape_size = TAPE_SIZE;
    context.tape_index = 0;

    return context;
}

void interpret(char* str)
{
    Context context = getContext();
    Instruction* instructions = parseString(str);
    
    executeInstructions(instructions, &context);
    free(context.tape);
    freeInstructions(instructions);
}

void executeInstructions(Instruction* instructions, Context* context)
{
    for (int i = 0; instructions[i].type != BF_END; ++i) {
        switch(instructions[i].type) {
        case BF_DATA:
            context->tape[context->tape_index] += instructions[i].value;
            break;

        case BF_PTR:
            context->tape_index += instructions[i].value;
            if (context->tape_index >= context->tape_size) {
                fprintf(stderr, "out of bound error.\n");
                exit(EXIT_FAILURE);
            }
            break;

        case BF_LOOP:
            while(context->tape[context->tape_index]) {
                executeInstructions(instructions[i].loop, context);
            }
            break;

        case BF_OUTPUT:
            for (int j = 0; j < instructions[i].value; ++j) {
                printf("%c", context->tape[context->tape_index]);
            }
            break;

        case BF_INPUT:
            for (int j = 0; j < instructions[i].value; ++j) {
                int c = getchar();
                if(c == EOF) {
                    fprintf(stderr, "Error reading input.\n");
                    exit(EXIT_FAILURE);
                }
                context->tape[context->tape_index] = c;
            }
            break;

        default:
            fprintf(stderr, "Invalid instruction type.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void freeInstructions(Instruction* instructions)
{
    for (int i = 0; instructions[i].type != BF_END; ++i) {
        if (instructions[i].type == BF_LOOP) freeInstructions(instructions[i].loop);
    }
    free(instructions);
}

