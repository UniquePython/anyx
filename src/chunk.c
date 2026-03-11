#include <stdlib.h>

#include "chunk.h"
#include "value.h"
#include "memory.h"

static void writeBytes(Chunk *chunk, uint64_t value, int bytes, int line)
{
    for (int i = bytes - 1; i >= 0; i--)
        writeChunk(chunk, (value >> (i * 8)) & 0xff, line);
}

void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(u8, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk *chunk, u8 byte, int line)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(u8, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int addConstant(Chunk *chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeConstant(Chunk *chunk, Value value, int line)
{
    u64 constant = addConstant(chunk, value);

    if (constant <= UINT8_MAX)
    {
        writeChunk(chunk, OP_CONSTANT_8, line);
        writeBytes(chunk, constant, 1, line);
    }
    else if (constant <= UINT16_MAX)
    {
        writeChunk(chunk, OP_CONSTANT_16, line);
        writeBytes(chunk, constant, 2, line);
    }
    else if (constant <= UINT32_MAX)
    {
        writeChunk(chunk, OP_CONSTANT_32, line);
        writeBytes(chunk, constant, 4, line);
    }
    else
    {
        writeChunk(chunk, OP_CONSTANT_64, line);
        writeBytes(chunk, constant, 8, line);
    }
}