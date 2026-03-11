#ifndef ANYX_CHUNK_H
#define ANYX_CHUNK_H

#include "common.h"

#include "value.h"

typedef enum
{
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct
{
    int count, capacity;
    u8 *code;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *);
void freeChunk(Chunk *);
void writeChunk(Chunk *, u8);
int addConstant(Chunk *, Value);

#endif