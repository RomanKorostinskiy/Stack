#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

enum Stack_errors
{
	STK_IS_NULL_PTR  = 1,
	DATA_IS_NULL_PTR = 2,
	STK_DESTROYED    = 4,
	STK_OVERFL       = 8,
	STK_UNDERFL      = 16,
	STK_DOUBLE_CTED  = 32,
    STRCT_CANARY_BAD = 64,
    DATA_CANARY_BAD  = 128,
    HASH_BAD         = 256,
};

#define FLOAT_DATA

#ifdef FLOAT_DATA
typedef double data_t;
#endif

#ifdef INT_DATA
typedef int data_t;
#endif

typedef size_t canary_t;
typedef size_t hash_t;


extern const int     START_CAPACITY;
extern const int     CAPACITY_STEP;
extern const size_t  CANARY_CONSTANT;
extern const data_t* UNAVAILABLE_ADR;

typedef struct Stack_t
{
    canary_t canary_left = 0;

	int      capacity = 0;
	data_t*  data = nullptr;
	int      size = 0;

    hash_t   hash = 0;

    canary_t canary_right = 0;
} Stack;

//TODO передавать функцию проверки, чтобы уменьшить количкество дефайнов
#define STACK_GENERAL_CHECK(check_function)             \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
    check_function;                                     \
                                                        \
    StackDump(stack, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
        return 1;                                       \
    }                                                   \
} while (0)

#define STACK_RESIZE_ERROR_CHECK()                      \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
	StackErrorCheck(stack);	                            \
                                                        \
    StackDump(stack, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
        return nullptr;	                                \
    }                                                   \
} while (0)


#define STACK_POP_ERROR_CHECK()                         \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
	StackErrorCheck(stack);				                \
                                                        \
    if (stack->size <= 0)                               \
    {                                                   \
        errors |= STK_UNDERFL;                          \
    }                                                   \
                                                        \
    StackDump(stack, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
		return (data_t) 0xBEDABEDA;	                    \
    }                                                   \
} while (0)


int StackCtor (Stack* stack, int capacity = START_CAPACITY);

int StackDtor (Stack* stack);

int StackPush (Stack* stack, data_t value);

data_t StackPop (Stack* stack);

data_t* StackResize (Stack* stack);

size_t StackHash (Stack* stack);

int StackErrorCheck (Stack* stack);

int StackCtorCheck(Stack* stack);

int StackDtorCheck (Stack* stack);

void StackDump (Stack* stack, const char* current_file, const char* current_function);

int StackTestInt (Stack* stack);

int StackTestFloat (Stack* stack);

void StackStructHack (Stack* stack);

void StackDataHack (Stack* stack);