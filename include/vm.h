#ifndef ANYX_VM_H
#define ANYX_VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 1024

typedef struct
{
    Chunk *chunk;
    u8 *ip;
    Value stack[STACK_MAX];
    Value *stackTop;
} VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM(VM *);
void freeVM(VM *);

InterpretResult interpret(VM *, Chunk *);

void push(VM *, Value);
Value pop(VM *);

#endif