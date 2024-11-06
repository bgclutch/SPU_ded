#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "../asm_headers/assembler.h"
#include "../asm_headers/putargs.h"
#include "../../../stack_ded/src/stack_headers/colorised_output.h"
#include "../../../lib_buffer_proc/buffer.h"
#include "../../../lib_file_proc/file.h"


Commands translation_func(const char* command_word, size_t wrd_size)
{
         if(strncmp(command_word, c_push, wrd_size) == 0) return PUSH;
    else if(strncmp(command_word, c_add,  wrd_size) == 0) return ADD;
    else if(strncmp(command_word, c_sub,  wrd_size) == 0) return SUB;
    else if(strncmp(command_word, c_mul,  wrd_size) == 0) return MUL;
    else if(strncmp(command_word, c_div,  wrd_size) == 0) return DIV;
    else if(strncmp(command_word, c_in,   wrd_size) == 0) return IN;
    else if(strncmp(command_word, c_out,  wrd_size) == 0) return OUT;
    else if(strncmp(command_word, c_sqrt, wrd_size) == 0) return SQRT;
    else if(strncmp(command_word, c_sin,  wrd_size) == 0) return SIN;
    else if(strncmp(command_word, c_cos,  wrd_size) == 0) return COS;
    else if(strncmp(command_word, c_dump, wrd_size) == 0) return DUMP;
    else if(strncmp(command_word, c_hlt,  wrd_size) == 0) return HLT;
    else if(strncmp(command_word, c_pop,  wrd_size) == 0) return POP;
    else if(strncmp(command_word, c_jmp,  wrd_size) == 0) return JMP;
    else if(strncmp(command_word, c_ja,   wrd_size) == 0) return JA;
    else if(strncmp(command_word, c_jae,  wrd_size) == 0) return JAE;
    else if(strncmp(command_word, c_jb,   wrd_size) == 0) return JB;
    else if(strncmp(command_word, c_jbe,  wrd_size) == 0) return JBE;
    else if(strncmp(command_word, c_je,   wrd_size) == 0) return JE;
    else if(strncmp(command_word, c_jne,  wrd_size) == 0) return JNE;
    else if(strncmp(command_word, c_call, wrd_size) == 0) return CALL;
    else if(strncmp(command_word, c_draw, wrd_size) == 0) return DRAW;
    else if(strncmp(command_word, c_ret,  wrd_size) == 0) return RET;
    else return MATVEY;
}

Registers choose_register(const char* command_word, size_t wrd_size)
{
         if(word_toupper_comparer(command_word, c_ax, wrd_size) == 0) return MY_AX;
    else if(word_toupper_comparer(command_word, c_bx, wrd_size) == 0) return MY_BX;
    else if(word_toupper_comparer(command_word, c_cx, wrd_size) == 0) return MY_CX;
    else return MY_DX;
}


Token_Type get_token_type(const char* name, size_t name_size)
{
         if(opcode_cmp(name, name_size))             return OPCODE;
    else if(arithm_operand_cmp(name, name_size))     return ARITHM;
    else if(push_pop_atr_cmp(name))                  return PFLAG;
    else if(number_cmp(name, name_size))             return NUMBER;
    else if(register_cmp(name, name_size))           return REGISTER;
    else if(memchr(name, ':', name_size) != nullptr) return LABEL_DEF;
    else                                             return LABEL_USE;
}


Cmp_Res opcode_cmp(const char* name, size_t name_size)
{
         if(strncmp(name, c_push, name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_add,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_sub,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_mul,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_div,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_in,   name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_out,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_sqrt, name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_sin,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_cos,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_dump, name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_hlt,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_pop,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_jmp,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_ja,   name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_jae,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_jb,   name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_jbe,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_je,   name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_jne,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_call, name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_ret,  name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_draw, name_size) == 0) return RIGHT_CMP;
    else return WRONG_CMP;
}


Cmp_Res number_cmp(const char* name, size_t name_size)
{
    size_t counter = 0;

    for(size_t i = 0; i < name_size; i++)
    {
        if(name[i] == '.' || name[i] == '-' || name[i] == '+' || isdigit(name[i]))
            counter++;
    }

    return counter == name_size ? RIGHT_CMP : WRONG_CMP ;
}


Cmp_Res register_cmp(const char* name, size_t name_size)
{
         if(word_toupper_comparer(name, c_ax, name_size) == 0) return RIGHT_CMP;
    else if(word_toupper_comparer(name, c_bx, name_size) == 0) return RIGHT_CMP;
    else if(word_toupper_comparer(name, c_cx, name_size) == 0) return RIGHT_CMP;
    else if(word_toupper_comparer(name, c_dx, name_size) == 0) return RIGHT_CMP;
    else return WRONG_CMP;
}


Arithm_Op choose_arithm_operation(const char* name, const size_t name_size)
{
         if(strncmp(name, c_add_op, name_size) == 0) return SUMM_BIT;
    else if(strncmp(name, c_mul_op, name_size) == 0) return MULTIPL_BIT;
    else return DEFAULT_BIT;
}

Cmp_Res arithm_operand_cmp(const char* name, const size_t name_size)
{
         if(strncmp(name, c_add_op, name_size) == 0) return RIGHT_CMP;
    else if(strncmp(name, c_mul_op, name_size) == 0) return RIGHT_CMP;
    else return WRONG_CMP;
}

Cmp_Res push_pop_atr_cmp(const char* name)
{
         if(name[0] == c_ramatrb) return RIGHT_CMP;
    else if(name[0] == c_ramatre) return RIGHT_CMP;
    else return WRONG_CMP;
}

Asm_Errors init_token_st(char* buffer, size_t file_size, Dynamic_Token* token_st)
{
    int all_bytes = 0;
    int cur_bytes = 0;

    while(all_bytes < (ssize_t)file_size)
    {
        // TODO check error
        Asm_Errors realloc_err = realloc_maker_token(token_st, realloc_if_up_needed_token(*token_st));

        ERROR_PROC(realloc_err);

        // fprintf(stderr, "token array size " BLUE_TEXT("%lu\n"), token_st->size);

        Token token_initializator = {};

        token_ctor(&token_initializator);

        char token_trns_name[32] = {};

        sscanf(buffer + all_bytes, "%s%n", token_trns_name, &cur_bytes);

        // fprintf(stderr, BLUE_TEXT("\ntoken name: %s && cur bytes: %d\n\n"), token_trns_name, cur_bytes);

        token_initializator.name_size = strlen(token_trns_name); // FIXME

        // fprintf(stderr, BLUE_TEXT("\ntoken name: %s && token name len: %lu\n\n"), token_trns_name, token_initializator.name_size);

        all_bytes += cur_bytes;

        token_initializator.name = buffer + all_bytes - token_initializator.name_size;


        // fprintf(stderr, "name " GREEN_TEXT("%s\n")"name size " BLUE_TEXT("%lu\n"), token_initializator.name,
        //                                                                            token_initializator.name_size);


        // fprintf(stderr, "all bytes " BLUE_TEXT("%d\n") "file size " BLUE_TEXT("%lu\n\n"), all_bytes, file_size);

        token_initializator.type = get_token_type(token_initializator.name,
                                                  token_initializator.name_size);

        switch(token_initializator.type)
        {
            case(OPCODE):
            {
                token_initializator.opcode = translation_func(token_initializator.name,
                                                              token_initializator.name_size);

                token_st->size++;
                break;
            }
            case(NUMBER):
            {
                token_initializator.number = strtod(token_initializator.name, NULL);
                token_st->size++;
                break;
            }
            case(LABEL_DEF):
            case(LABEL_USE):
            {
                token_st->size++;
                break;
            }
            case(REGISTER):
            {
                token_initializator.register_num = choose_register(token_initializator.name,
                                                                   token_initializator.name_size);
                token_st->size++;
                break;
            }
            case(PFLAG):
            {
                token_st->size++;
                break;
            }
            case(ARITHM):
            {
                token_initializator.operation = choose_arithm_operation(token_initializator.name,
                                                                        token_initializator.name_size);
                token_st->size++;
                break;
            }
            case(ERROR):
            {
                return TOKEN_INIT_ERR;
            }
            default:
            {
                assert(0 && "add some changes in init token struct or check it maybe");
                return TOKEN_INIT_ERR;
            };
        };

        token_st->token_array[token_st->size - 1] = token_initializator;
    }

    // token_dump(token_st, stderr);

    return ALL_OK_RETURN;
}


Asm_Errors ctor_dyn_token(Dynamic_Token* token_st)
{
    token_st->capacity = c_token_alloc_coef;
    token_st->size = 0;
    token_st->token_array = (Token*) calloc(token_st->capacity, sizeof(Token));

    if(token_st->token_array == nullptr)
        return DYN_TKN_CTOR_ERR;

    for(size_t i = 0; i < token_st->capacity; i++)
    {
        token_ctor(&token_st->token_array[i]);
    }

    return ALL_OK_RETURN;
}


Asm_Errors dtor_dyn_token(Dynamic_Token* token_st)
{
    token_st->capacity = 0;
    token_st->size = 0;
    free(token_st->token_array);
    token_st->token_array = nullptr;
    if(token_st->token_array != nullptr)
        return DYN_TKN_DTOR_ERR;

    return ALL_OK_RETURN;
}


Asm_Errors realloc_maker_token(Dynamic_Token* token_st, size_t new_capacity)
{
    // fprintf(stderr, "capacity old (realloc) %lu\n" ,token_st->capacity);

    if(new_capacity == token_st->capacity)
        return ALL_OK_RETURN;

    Token* new_array = (Token*) realloc(token_st->token_array, new_capacity * sizeof(Token));

    if(new_array == nullptr)
    {
        fprintf(stderr, "stack RED_TEXT(couldn't realloced) at address:YELLOW_TEXT(%p\n)", new_array);
        return REALLOC_ERROR;
    }


    token_st->capacity    = new_capacity;
    token_st->token_array = new_array;

    for(size_t i = token_st->size; i < token_st->capacity; i++)
    {
        token_ctor(&token_st->token_array[i]);
    }

    // fprintf(stderr, "capacity (realloc) %lu\n\n", token_st->capacity);

    return ALL_OK_RETURN;

}


size_t realloc_if_up_needed_token(Dynamic_Token token_st)
{
    if(token_st.capacity - token_st.size <= 1)
        return token_st.capacity * c_realloc_up_coef;
    else
        return token_st.capacity;
}


size_t realloc_if_down_needed_token(Dynamic_Token token_st)
{
    if(c_realloc_down_coef * 2 <= token_st.size && token_st.size * c_realloc_down_coef <= token_st.capacity)
        return token_st.capacity / c_realloc_down_coef;
    else
        return token_st.capacity;
}

#define WRITE_(...) if (translator_flag == SECOND_RUN) __VA_ARGS__

Asm_Errors translator(Dynamic_Token* token_st, FILE* file_dump, Label* labels_array, const Run_Flags translator_flag,
                      size_t* instruction_pointer, FILE* file_write)
{
    assert(token_st);
    assert(token_st->token_array);
    assert(file_dump);
    assert(file_write);
    assert(labels_array);
    assert(translator_flag == FIRST_RUN || translator_flag == SECOND_RUN);


    size_t buffer_size = 0;
    size_t prefix_sum = 0;

    char* buffer = nullptr;
    if (translator_flag == SECOND_RUN) {
        buffer_size = *instruction_pointer + sizeof(*instruction_pointer);
        buffer = buffer_create_raw(buffer_size, sizeof(char));
        if(buffer == nullptr)
            return BUFFER_NULLPTR;

        memcpy(buffer, instruction_pointer, sizeof(*instruction_pointer));
        prefix_sum += sizeof(*instruction_pointer);
    }

    for(size_t ind = 0; ind < token_st->size; ind++)
    {
        switch(token_st->token_array[ind].type)
        {
            case OPCODE:
            {
                // TODO make func

                WRITE_(memcpy(buffer + prefix_sum, &token_st->token_array[ind].opcode,
                              sizeof(char)));

                prefix_sum += sizeof(char);

                if(strncmp(token_st->token_array[ind].name, c_push, token_st->token_array[ind].name_size) == 0)
                {
                    if (translator_flag == SECOND_RUN)
                    {
                        char push_arg = putarg_push(token_st, ind);
                        if(push_arg == 0)
                        {
                            fprintf(stderr, "error in 2 run in push arg\n");
                            free(buffer);
                            return PUSH_ARG_ERR;
                        }
                        memcpy(buffer + prefix_sum, &push_arg, sizeof(push_arg));
                    }
                    prefix_sum += sizeof(char);
                }
                else if(strncmp(token_st->token_array[ind].name, c_pop, token_st->token_array[ind].name_size) == 0)
                {
                    if (translator_flag == SECOND_RUN)
                    {
                        char pop_arg = putarg_pop(token_st, ind);
                        if(pop_arg == 0)
                        {
                            fprintf(stderr, "error in 2 run in pop arg");
                            free(buffer);
                            return POP_ARG_ERR;
                        }
                        memcpy(buffer + prefix_sum, &pop_arg, sizeof(pop_arg));
                    }
                    prefix_sum += sizeof(char);
                }

                break;
            }
            case REGISTER:
            {
                WRITE_(memcpy(buffer + prefix_sum, &token_st->token_array[ind].register_num,
                                                   sizeof(char)));
                prefix_sum += sizeof(char);
                break;
            }
            case NUMBER:
            {
                WRITE_(memcpy(buffer + prefix_sum, &token_st->token_array[ind].number,
                                             sizeof(StackElem_t)));
                prefix_sum += sizeof(StackElem_t);

                break;
            }
            case LABEL_DEF:
            {
                // TODO make func
                if(translator_flag == FIRST_RUN) // if token type == label
                {
                    Asm_Errors label_pos_err = put_new_label(token_st, labels_array, prefix_sum,
                                                             ind, c_label_num);
                    ERROR_PROC(label_pos_err);
                }
                break;
            }
            case LABEL_USE:
            {
                if (translator_flag == FIRST_RUN)
                {
                    prefix_sum += sizeof(short);
                }
                else // SECOND_RUN
                {
                    Asm_Errors put_label_err = put_label_to_buffer(token_st, labels_array, ind, c_label_num,
                                                                   buffer, &prefix_sum);
                    ERROR_PROC(put_label_err);
                }
                break;
            }
            case PFLAG: // FIXME syntax_err
            {
                assert(0);
                if(translator_flag != SECOND_RUN)
                {
                    *instruction_pointer += token_st->token_array[ind].token_size;
                }
                break;
            }
            case ARITHM: // FIXME syntax_err
            {
                assert(0);
                if(translator_flag != SECOND_RUN)
                {
                    *instruction_pointer += token_st->token_array[ind].token_size;
                    break;
                }
                break;
            }
            case ERROR:
            {
                free(buffer);
                return TRANSLATOR_ERR;
            }
            default:
            {
                assert(0 && "add some changes");
            };
        };
    }


    if(translator_flag == SECOND_RUN)
    {
        fwrite(buffer, sizeof(char), buffer_size, file_write);
        free(buffer);
        buffer = nullptr;
    }
    else
    {
        *instruction_pointer = prefix_sum;
    }

    return ALL_OK_RETURN;
}
#undef WRITE_


Asm_Errors ctor_labels(Label* labels_array, size_t size)
{
    for(size_t index = 0; index < size; index++)
    {
        labels_array[index].name = c_basic_name;
        labels_array[index].name_size = c_b_name_len;
        labels_array[index].ip = c_label_ip_null;
    }
    return ALL_OK_RETURN;
}


int word_toupper_comparer(const char* str_1, const char* str_2, size_t len_1)
{
    size_t ind_str_1 = 0;
    size_t ind_str_2 = 0;
    int result = 0;

    while((ind_str_1 < len_1) && !isalpha(str_1[ind_str_1]))
    {
        ind_str_1++;
    }
    while((ind_str_2 < len_1) && !isalpha(str_2[ind_str_2]))
    {
        ind_str_2++;
    }

    while(ind_str_1 < len_1 && ind_str_2 < len_1 && result == 0)
    {
        result = toupper(str_1[ind_str_1]) - toupper(str_2[ind_str_2]);
        ind_str_1++;
        ind_str_2++;
    }
    return result;
}


void label_dump(Label* labels_array, size_t labels_num, FILE* file)
{
    for(size_t i = 0; i < labels_num; i++)
    {
        fprintf(file, "array struct elem %lu: jump address:%ld", i, labels_array[i].ip);
        fprintf(file, "\n");
    }

    fprintf(file, "\n\n");
}


void token_dump(Dynamic_Token* token, FILE* file)
{
    for(size_t i = 0; i < token->size; i++)
    {
        fprintf(file, "name:");

        for(size_t j = 0; j < token->token_array[i].name_size; j++)
        {
            fprintf(file, "%c", token->token_array[i].name[j]);
        }


        fprintf(file,
                        "\nname size %lu\n"
                        "token_size %lu\n"
                        "type %d\n"
                        "opcode %d\n"
                        "register %d\n"
                        "label address %ld\n"
                        "number %lg\n"
                        "arithmetic operation %d\n",
                        token->token_array[i].name_size,
                        token->token_array[i].token_size,
                        token->token_array[i].type,
                        token->token_array[i].opcode,
                        token->token_array[i].register_num,
                        token->token_array[i].label_address,
                        token->token_array[i].number,
                        token->token_array[i].operation);
        fprintf(file, "\n\n");
    }
}


Asm_Errors token_ctor(Token* token)
{
    token->name = c_basic_name;
    token->name_size = c_b_name_len;
    token->token_size = 0;
    token->type = ERROR;
    token->opcode = MATVEY;
    token->register_num = REGERR;
    token->label_address = c_label_ip_null;
    token->number = -1;
    token->operation = DEFAULT_BIT;

    return ALL_OK_RETURN;
}


Asm_Errors label_to_null(Label* labels_array, size_t index)
{
    labels_array[index].name = c_basic_name;
    labels_array[index].name_size = c_b_name_len;
    labels_array[index].ip = c_label_ip_null;
    return ALL_OK_RETURN;
}


Asm_Errors put_new_label(Dynamic_Token* token_st, Label* labels_array, size_t pref_sum, const size_t ind,
                         const size_t labels_arr_len)
{
    for(size_t pos = 0; pos < labels_arr_len; pos++) // iteration by labels array
    {
        if(strncmp(labels_array[pos].name, c_basic_name, c_b_name_len) == 0) // find first free spot
        {

            labels_array[pos].name = token_st->token_array[ind].name;
            labels_array[pos].name_size = token_st->token_array[ind].name_size;

            labels_array[pos].ip = (int64_t)pref_sum; // else address is -1 (it will be checked in second run)
            token_st->token_array[ind].label_address = (int64_t)pref_sum;

            return ALL_OK_RETURN;
        }
    }

    return TOO_MANY_LBLS;
}


Asm_Errors put_label_to_buffer(Dynamic_Token* token_st, Label* labels_array, const size_t token_ind,
                              const size_t labels_arr_len, char* buffer, size_t* prefix_sum)
{
    for(size_t i = 0; i < labels_arr_len; i++)
    {
        if(strncmp(token_st->token_array[token_ind].name, labels_array[i].name, token_st->token_array->name_size - 1) == 0)
        {
            if(labels_array[i].ip == c_label_ip_null)
            {
                fprintf(stderr, "wrong ip!\n");
                fprintf(stderr, RED_TEXT("error here: %s\n"), labels_array->name);
                return LBL_TO_BUF_ERR;
            }

            // label_dump(labels_array, labels_arr_len, stderr);

            memcpy(buffer + *prefix_sum, &labels_array[i].ip, sizeof(short));

            *prefix_sum += sizeof(short);

            return ALL_OK_RETURN;
        }
    }

    return GET_LBL_POS_ERR;
}


int err_checker(const Asm_Errors error_code, const char* name, const size_t line)
{
    switch(error_code)
    {
        case SYNT_ERR:
        {
            fprintf(stderr, RED_TEXT("\nSYNTAX ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case TOKEN_INIT_ERR:
        {
            fprintf(stderr, RED_TEXT("\nTOKEN INITIALISATION ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case REALLOC_ERROR:
        {
            fprintf(stderr, RED_TEXT("\nREALLOC ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case TRANSLATOR_ERR:
        {
            fprintf(stderr, RED_TEXT("\nTRANSLATION ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case LBL_TO_BUF_ERR:
        {
            fprintf(stderr, RED_TEXT("\nLABEL TRANSLATION ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case GET_BINCODE_ERR:
        {
            fprintf(stderr, RED_TEXT("\nGET BINCODE ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case BIN_TO_FILE_ERR:
        {
            fprintf(stderr, RED_TEXT("\nBIN TO FILE TRANSLATION ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case FREE_ALL_ERR:
        {
            fprintf(stderr, RED_TEXT("\nFREE ALL ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case DYN_TKN_CTOR_ERR:
        {
            fprintf(stderr, RED_TEXT("\nCTOR DYNAMIC TOKEN ARRAY ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case DYN_TKN_DTOR_ERR:
        {
            fprintf(stderr, RED_TEXT("\nDTOR DYNAMIC TOKEN ARRAY ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case PUSH_ARG_ERR:
        {
            fprintf(stderr, RED_TEXT("\nPUSH ARG ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case POP_ARG_ERR:
        {
            fprintf(stderr, RED_TEXT("\nPOP ARG ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case GET_LBL_POS_ERR:
        {
            fprintf(stderr, RED_TEXT("\nGET LABEL POSITION ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case BUFFER_NULLPTR:
        {
            fprintf(stderr, RED_TEXT("\nBUFFER ON NULLPTR ERROR here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case TOO_MANY_LBLS:
        {
            fprintf(stderr, RED_TEXT("\nTOO MANY LABELS here %s:%lu\n"), name, line);
            return 0;
            break;
        }
        case ALL_OK_RETURN:
        {
            return 1;
            break;
        }
        default:
        {
            assert(0 && "err checker assert");
        };
    };
}



Asm_Errors get_bincode(const char* first_filename, const char* second_filename)
{
    FILE* file_read = nullptr;
    Files_Errors file_r_open_err = file_read_open(&file_read, first_filename);

    if(file_r_open_err != ALL_GOOD_RET_F)
        return GET_BINCODE_ERR;

    size_t file_size = symbols_number(file_read);

    if(!res_checker((ssize_t)file_size, __LINE__, __FILE__))
        return GET_BINCODE_ERR;


    char* buffer = buffer_create(file_size, sizeof(char), file_read);

    if(!res_checker((ssize_t)buffer, __LINE__, __FILE__))
    {
        free(buffer);
        return GET_BINCODE_ERR;
    }

    Files_Errors f_read_res = file_close(file_read);

    if(f_read_res != ALL_GOOD_RET_F)
    {
        free(buffer);
        return GET_BINCODE_ERR;
    }

    Asm_Errors create_labels_err = assesmler_process(buffer, file_size, second_filename);

    if(!ERROR_PROC(create_labels_err))
    {
        free(buffer);
        return GET_BINCODE_ERR;
    }

    free(buffer);
    buffer = nullptr;

    return ALL_OK_RETURN;
}


Asm_Errors bin_code_put_to_file(const char* out_filename, Dynamic_Token* tokens, Label* labels_array)
{
    assert(out_filename);
    assert(tokens);
    assert(labels_array);

    FILE* file_write = nullptr;
    Files_Errors file_w_open_err = file_write_open(&file_write, out_filename);

    if(file_w_open_err != ALL_GOOD_RET_F)
        return BIN_TO_FILE_ERR;


    FILE* dumpik = nullptr;
    Files_Errors file_dump_open_err = file_write_open(&dumpik, "dumps/dump_file.txt"); // TODO make const


    if(file_dump_open_err != ALL_GOOD_RET_F) {
        file_close(file_write);
        return BIN_TO_FILE_ERR;
    }

    size_t calloc_proc_len = 0;
    Asm_Errors translator_first_run_err = translator(tokens, dumpik, labels_array, FIRST_RUN, &calloc_proc_len, file_write);

    if(!ERROR_PROC(translator_first_run_err))
    {
        file_close(file_write);
        file_close(dumpik);
        return BIN_TO_FILE_ERR;
    }

    Asm_Errors translator_second_run_err = translator(tokens, dumpik, labels_array, SECOND_RUN, &calloc_proc_len, file_write);

    label_dump(labels_array, c_label_num, dumpik);
    token_dump(tokens, dumpik);

    if(!ERROR_PROC(translator_second_run_err))
    {
        file_close(file_write);
        file_close(dumpik);
        return BIN_TO_FILE_ERR;
    }

    Files_Errors f_write_cl = file_close(file_write);

    Files_Errors f_dump_cl = file_close(dumpik);

    if(f_write_cl != ALL_GOOD_RET_F || f_dump_cl != ALL_GOOD_RET_F)
        return BIN_TO_FILE_ERR;

    return ALL_OK_RETURN;
}



Asm_Errors assesmler_process(char* buffer, const size_t file_size, const char* second_filename)
{
    Label labels_array[c_label_num] = {};

    Asm_Errors ctor_labels_arr_result = ctor_labels(labels_array, c_label_num);

    ERROR_PROC(ctor_labels_arr_result);

    Dynamic_Token token = {};
    Asm_Errors ctor_dyn_token_result = ctor_dyn_token(&token);

    if(ERROR_PROC(ctor_dyn_token_result) == 0)
    {
        dtor_dyn_token(&token);
        return DYN_TKN_CTOR_ERR;
    }

    Asm_Errors init_token_err = init_token_st(buffer, file_size, &token);

    if(!ERROR_PROC(init_token_err))
    {
        dtor_dyn_token(&token);
        return TOKEN_INIT_ERR;
    }

    Asm_Errors bin_put_to_file_err = bin_code_put_to_file(second_filename, &token, labels_array);

    if(!ERROR_PROC(bin_put_to_file_err))
    {
        dtor_dyn_token(&token);
        return BIN_TO_FILE_ERR;
    }

    ERROR_PROC(dtor_dyn_token(&token));

    return ALL_OK_RETURN;
}
