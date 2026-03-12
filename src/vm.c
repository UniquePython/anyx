#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

static void resetStack(VM *vm)
{
    vm->stackTop = vm->stack;
}

void initVM(VM *vm)
{
    resetStack(vm);
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
        printf("          ");
        for (Value *slot = vm->stack; slot < vm->stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif
        u8 instruction = READ_BYTE();

        switch (instruction)
        {
        case OP_CONSTANT_8:
        {
            u64 index = READ_BYTES(1);
            push(vm, vm->chunk->constants.values[index]);
            break;
        }

        case OP_CONSTANT_16:
        {
            u64 index = READ_BYTES(2);
            push(vm, vm->chunk->constants.values[index]);
            break;
        }

        case OP_CONSTANT_32:
        {
            u64 index = READ_BYTES(4);
            push(vm, vm->chunk->constants.values[index]);
            break;
        }

        case OP_CONSTANT_64:
        {
            u64 index = READ_BYTES(8);
            push(vm, vm->chunk->constants.values[index]);
            break;
        }

        case OP_RETURN:
            printValue(pop(vm));
            printf("\n");
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

void push(VM *vm, Value value)
{
    *vm->stackTop = value;
    vm->stackTop++;
}

Value pop(VM *vm)
{
    vm->stackTop--;
    return *vm->stackTop;
}