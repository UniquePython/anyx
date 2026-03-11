#ifndef ANYX_VM_H
#define ANYX_VM_H

#include "chunk.h"

typedef struct
{
    Chunk *chunk;
    u8 *ip;
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

#endif