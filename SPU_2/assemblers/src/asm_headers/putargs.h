#ifndef PUTARGS_H_
#define PUTARGS_H_

#include <stdio.h>
#include "assembler.h"


char putarg_push(Dynamic_Token* token, size_t ind);

char putarg_pop(Dynamic_Token* token, size_t ind);

int swap_tokens(Token* token_1, Token* token_2);


#endif // PUTARGS_H_