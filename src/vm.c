#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

void initVM(VM *vm)
{
}

void freeVM(VM *vm)
{
}

static u64 read_n(VM *vm, int n)
{
    u64 value = 0;

    for (int i = 0; i < n; i++)
        value = (value << 8) | (*vm->ip++);

    return value;
}

static InterpretResult run(VM *vm)
{
#define READ_BYTE() (*vm->ip++)
#define READ_BYTES(n) (read_n(vm, (n)))

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif
        u8 instruction = READ_BYTE();

        switch (instruction)
        {
        case OP_CONSTANT_8:
        {
            u64 index = READ_BYTES(1);
            printValue(vm->chunk->constants.values[index]);
            printf("\n");
            break;
        }

        case OP_CONSTANT_16:
        {
            u64 index = READ_BYTES(2);
            printValue(vm->chunk->constants.values[index]);
            printf("\n");
            break;
        }

        case OP_CONSTANT_32:
        {
            u64 index = READ_BYTES(4);
            printValue(vm->chunk->constants.values[index]);
            printf("\n");
            break;
        }

        case OP_CONSTANT_64:
        {
            u64 index = READ_BYTES(8);
            printValue(vm->chunk->constants.values[index]);
            printf("\n");
            break;
        }

        case OP_RETURN:
            return INTERPRET_OK;
        }
    }

#undef READ_BYTES
#undef READ_BYTE
}

InterpretResult interpret(VM *vm, Chunk *chunk)
{
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return run(vm);
}