#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

static void resetStack(VM *vm)
{
    vm->stackTop = vm->stack;
}

void initVM(VM *vm)
{
    vm->stackCapacity = STACK_INITIAL_CAPACITY;
    vm->stack = malloc(vm->stackCapacity * sizeof(Value));
    vm->stackTop = vm->stack;
    vm->chunk = NULL;
    vm->ip = NULL;
}

void freeVM(VM *vm)
{
    free(vm->stack);
    vm->stack = NULL;
    vm->stackTop = NULL;
    vm->stackCapacity = 0;
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

#define BINARY_OP(vm, op)   \
    do                      \
    {                       \
        double b = pop(vm); \
        double a = pop(vm); \
        push(vm, a op b);   \
    } while (0)

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

        case OP_ADD:
            BINARY_OP(vm, +);
            break;

        case OP_SUBTRACT:
            BINARY_OP(vm, -);
            break;

        case OP_MULTIPLY:
            BINARY_OP(vm, *);
            break;

        case OP_DIVIDE:
            BINARY_OP(vm, /);
            break;

        case OP_NEGATE:
            push(vm, -pop(vm));
            break;

        case OP_RETURN:
            printValue(pop(vm));
            printf("\n");
            return INTERPRET_OK;
        }
    }

#undef BINARY_OP

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
    if ((size_t)(vm->stackTop - vm->stack) >= vm->stackCapacity)
    {
        size_t oldCapacity = vm->stackCapacity;
        vm->stackCapacity *= 1.5;
        size_t offset = vm->stackTop - vm->stack;

        vm->stack = realloc(vm->stack, vm->stackCapacity * sizeof(Value));
        if (!vm->stack)
        {
            fprintf(stderr, "Failed to allocate memory for VM stack.\n");
            exit(1);
        }

        vm->stackTop = vm->stack + offset;
    }

    *vm->stackTop++ = value;
}

Value pop(VM *vm)
{
    if (vm->stackTop == vm->stack)
    {
        fprintf(stderr, "Stack underflow.\n");
        exit(1);
    }

    return *--vm->stackTop;
}