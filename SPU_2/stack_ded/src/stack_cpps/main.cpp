#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../stack_headers/stack.h"
#include "../stack_headers/colorised_output.h"


int main(void)
{
    Main_Stack_Struct stack_data = {};

    StackElem_t elem = 0;

    StackElem_t test[] = {1.1, 11, 121, 12312, 1231312.5675, 124342, 1, 1, 1, 1, 1, 1};

    ASSERT_FUNC(DEBUG_VAR(!)ctor_stack(&stack_data));

    for(size_t i = 0; i < sizeof(test) / sizeof(StackElem_t); i++)
    {
        elem = test[i];
        Error_Codes push_result = stack_push(&stack_data, elem);

        if(push_result == STACK_NOT_REALLOCED)
        {
            fprintf(stderr, "BLUE_TEXT(stack couldn't recalloc up\n)"); // add choose option????
            return 0;
        }

        ASSERT(!push_result && "push result error") DEBUG_VAR(;)
    }

    for(size_t i = stack_data.size; i >= 1; i--)
    {
        Error_Codes pop_result = stack_pop(&stack_data, &elem);

        if(pop_result == STACK_NOT_REALLOCED)
        {
            fprintf(stderr, "BLUE_TEXT(stack couldn't recalloc down\n)");
            return 0;
        }

        ASSERT(!pop_result && "pop result error") DEBUG_VAR(;)
    }

    ASSERT_FUNC(DEBUG_VAR(!)dtor_stack(&stack_data));

    DEBUG_VAR
    (if(fclose(stack_data.dump_file) != 0)
        printf("ty eblan?\n");)

    return 0;
}