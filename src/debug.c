#include <stdio.h>
#include <inttypes.h>

#include "debug.h"
#include "value.h"

static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstructionN(const char *name, Chunk *chunk, int offset, int bytes)
{
    u64 constant = 0;

    for (int i = 0; i < bytes; i++)
    {
        constant <<= 8;
        constant |= chunk->code[offset + 1 + i];
    }

    printf("%-16s %4" PRIu64 " '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");

    return offset + 1 + bytes;
}

void disassembleChunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;)
        offset = disassembleInstruction(chunk, offset);
}

int disassembleInstruction(Chunk *chunk, int offset)
{
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
        printf("   |-> ");
    else
        printf("%4d ", chunk->lines[offset]);

    u8 instruction = chunk->code[offset];
    switch (instruction)
    {

    case OP_CONSTANT_8:
        return constantInstructionN("OP_CONSTANT_8", chunk, offset, 1);

    case OP_CONSTANT_16:
        return constantInstructionN("OP_CONSTANT_16", chunk, offset, 2);

    case OP_CONSTANT_32:
        return constantInstructionN("OP_CONSTANT_32", chunk, offset, 4);

    case OP_CONSTANT_64:
        return constantInstructionN("OP_CONSTANT_64", chunk, offset, 8);

    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);

    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);

    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);

    case OP_DIVIDE:
        return simpleInstruction("OP_DIVIDE", offset);

    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);

    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);

    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}