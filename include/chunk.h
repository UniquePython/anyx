#ifndef ANYX_CHUNK_H
#define ANYX_CHUNK_H

#include "common.h"

#include "value.h"

typedef enum
{
    OP_CONSTANT_8,
    OP_CONSTANT_16,
    OP_CONSTANT_32,
    OP_CONSTANT_64,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct
{
    int count, capacity;
    u8 *code;
    int *lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *);
void freeChunk(Chunk *);
void writeChunk(Chunk *, u8, int);

int addConstant(Chunk *, Value);
void writeConstant(Chunk *, Value, int);

#endif