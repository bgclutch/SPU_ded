#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "../asm_headers/putargs.h"
#include "../../../lib_buffer_proc/buffer.h"
#include "../../../lib_file_proc/file.h"



char putarg_push(Dynamic_Token* token, size_t ind)
{
    ind++; // first incr
    char push_dest_byte = 0;

    switch(token->token_array[ind].type) // check first token type after push, many branches
    {
        case(PFLAG):
        {
            if(token->token_array[ind].name[0] != c_ramatrb && token->token_array[ind + 4].name[0] != c_ramatre)
            {
                fprintf(stderr, "add '[' ']' to your push\n");
                assert(0);
            }

            push_dest_byte |= push_dest_byte | RAM_BIT;

            ind++; //second incr

            if(token->token_array[ind].type == REGISTER)
            {
                push_dest_byte |= REGISTER_BIT;

                ind++; // third incr

                if(token->token_array[ind].type == ARITHM) // first branch
                {
                    push_dest_byte |= (char)token->token_array[ind].operation;

                    ind++; // fourth incr

                    if(token->token_array[ind].type != NUMBER)
                    {
                        fprintf(stderr, "not allowed second register\n");
                        assert(0);
                    }

                    push_dest_byte |= NUMBER_BIT;
                }
            }
            else if(token->token_array[ind].type == NUMBER)
            {
                push_dest_byte |= NUMBER_BIT;

                ind++; // third incr

                if(token->token_array[ind].type == ARITHM) // second branch
                {
                    push_dest_byte |= (char)token->token_array[ind].operation;

                    ind++; // fourth incr

                    if(token->token_array[ind].type != REGISTER)
                    {
                        fprintf(stderr, "not allowed second number\n");
                        assert(0);
                    }

                    push_dest_byte |= REGISTER_BIT;

                    swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
                }
            }

            return push_dest_byte;

            break;
        }
        case(REGISTER):
        {
            push_dest_byte |= REGISTER_BIT;

            ind++; // second incr

            if(token->token_array[ind].type == ARITHM) // first branch
            {
                push_dest_byte |= (char)token->token_array[ind].operation;

                ind++; // third incr

                if(token->token_array[ind].type != NUMBER)
                {
                    fprintf(stderr, "not allowed second register\n");
                    assert(0);
                }

                push_dest_byte |= NUMBER_BIT;
            }

            return push_dest_byte;

            break;
        }
        case(NUMBER):
        {
            push_dest_byte |= NUMBER_BIT;

            ind++; // third incr

            if(token->token_array[ind].type == ARITHM) // first branch
            {
                push_dest_byte |= (char)token->token_array[ind].operation;

                ind++; // fourth incr

                if(token->token_array[ind].type != REGISTER)
                {
                    fprintf(stderr, "not allowed second number\n");
                    assert(0);
                }

                push_dest_byte |= REGISTER_BIT;

                swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
            }

            return push_dest_byte;

            break;
        }
        case(OPCODE):
        {
            assert(0 && "opcode push");
            break;
        }
        case(LABEL_USE):
        case(LABEL_DEF):
        {
            assert(0 && "label push");
            break;
        }
        case(ARITHM):
        {
            assert(0 && "arithm push");
            break;
        }
        case(ERROR):
        {
            assert(0 && "error push");
            break;
        }
        default:
        {
            assert(0 && "assert in putarg push");
            break;
        }
    }
}


char putarg_pop(Dynamic_Token* token, size_t ind)
{
    ind++;

    char pop_dest_byte = 0;

    switch(token->token_array[ind].type)
    {
        case(PFLAG):
        {
            if(token->token_array[ind].name[0] != c_ramatrb && (token->token_array[ind + 4].name[0] != c_ramatre ||
                                                                token->token_array[ind + 2].name[0] != c_ramatre))
            {
                fprintf(stderr, "add '[' ']' to your pop\n");
                assert(0); // FIXME CHANGE KAKASHECHKU?
            }

            pop_dest_byte |= pop_dest_byte | RAM_BIT;

            ind++;

            if(token->token_array[ind].type == NUMBER)
            {
                pop_dest_byte |= pop_dest_byte | NUMBER_BIT;

                ind++;

                if(token->token_array[ind].type == ARITHM)
                {
                    pop_dest_byte |= pop_dest_byte | (char)token->token_array[ind].operation;

                    ind++;

                    if(token->token_array[ind].type != REGISTER)
                    {
                        fprintf(stderr, "not allowed 2 numbers\n");
                        assert(0 && "remake it\n");
                    }

                    pop_dest_byte |= pop_dest_byte | REGISTER_BIT;

                    swap_tokens(&token->token_array[ind], &token->token_array[ind - 2]);
                }
            }
            else if(token->token_array[ind].type == REGISTER)
            {
                pop_dest_byte |= pop_dest_byte | REGISTER_BIT;

                ind++;

                if(token->token_array[ind].type != ARITHM)
                {
                    return pop_dest_byte;
                    // fprintf(stderr, "dobav' arithmetic operation to pop pls\n");
                    // assert(0 && "pososi moy huy\n");
                }

                pop_dest_byte |= pop_dest_byte | (char)token->token_array[ind].operation;

                ind++;

                if(token->token_array[ind].type != NUMBER)
                {
                    fprintf(stderr, "not allowed second register\n");
                    assert(0);
                }

                pop_dest_byte |= pop_dest_byte | NUMBER_BIT;
            }

            return pop_dest_byte;

            break;
        }
        case(REGISTER):
        {
            return REGISTER_BIT;
            break;
        }
        case(OPCODE):
        {
            assert(0 && "opcode pop");
            break;
        }
        case(LABEL_USE):
        case(LABEL_DEF):
        {
            assert(0 && "label pop");
            break;
        }
        case(ARITHM):
        {
            assert(0 && "arithm pop");
            break;
        }
        case(ERROR):
        {
            assert(0 && "error pop");
            break;
        }
        case(NUMBER):
        {
            assert(0 && "number pop");
            break;
        }
        default:
        {
            assert(0 && "write better pls(\n");
            break;
        }
    }
}


int swap_tokens(Token* token_1, Token* token_2)
{
    Token temp = {};

    temp = *token_2;
    *token_2 = *token_1;
    *token_1 = temp;

    return 1;
}