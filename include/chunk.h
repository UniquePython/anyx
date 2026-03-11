#ifndef ANYX_CHUNK_H
#define ANYX_CHUNK_H

#include "common.h"

typedef enum
{
    OP_RETURN,
} OpCode;

typedef struct
{
    int count, capacity;
    u8 *code;
} Chunk;

void initChunk(Chunk *);
void freeChunk(Chunk *);
void writeChunk(Chunk *, u8);

#endif