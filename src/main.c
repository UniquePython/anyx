#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

int main(int argc, const char *argv[])
{
    VM vm;
    initVM(&vm);

    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 300; i++)
        writeConstant(&chunk, i, 1);

    writeChunk(&chunk, OP_RETURN, 5);

    disassembleChunk(&chunk, "test chunk");

    interpret(&vm, &chunk);

    freeVM(&vm);
    freeChunk(&chunk);
    return 0;
}