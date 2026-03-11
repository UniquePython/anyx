#ifndef ANYX_DEBUG_H
#define ANYX_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk *, const char *);
int disassembleInstruction(Chunk *, int);

#endif