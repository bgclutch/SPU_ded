#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../processor_headers/processor.h"
#include "../../../stack_ded/src/stack_headers/stack.h"
#include "../../../stack_ded/src/stack_headers/colorised_output.h"
#include "../../../lib_buffer_proc/buffer.h"



Error_Codes_SPU ctor_proc(Struct_SPU *SPU, const size_t buffer_size, FILE* file)
{
    SPU->execode = nullptr;
    memset(SPU->registers, 0, sizeof(SPU->registers[0]) * REGCOUNT);
    ctor_stack(&SPU->stack);
    ctor_stack(&SPU->cllstk);
    SPU->ip = 0;
    SPU->flag = 1;
    for(size_t i = 0; i < RAM_SIZE; i++)
    {
        SPU->RAM[i] = 0;
    }

    SPU->execode = (uint8_t*)buffer_create(buffer_size, sizeof(char), file);

    if(SPU->execode == nullptr)
    {
        free(SPU->execode);
        SPU->execode = nullptr;
        return CTOR_ERR;
    }

    return NORMAL_COND;
}

Error_Codes_SPU dtor_proc(Struct_SPU *SPU)
{
    free(SPU->execode);
    SPU->execode = nullptr;
    memset(SPU->registers, 0, sizeof(SPU->registers[0]) * REGCOUNT);
    dtor_stack(&SPU->stack);
    dtor_stack(&SPU->cllstk);
    SPU->ip = 0;
    SPU->flag = 0;
    for(size_t i = 0; i < RAM_SIZE; i++)
    {
        SPU->RAM[i] = 0;
    }

    if(SPU->execode != nullptr)
        return DTOR_ERR;

    return NORMAL_COND;
}


Opcode_Errors push_maker(Struct_SPU* spu) // FIXME хуйня переделывай
{
    if(spu->execode[spu->ip] != PUSH)
    {
        fprintf(stderr, "here's not a push!\n");
        return WRONG_OPCODE;
    }

    spu->ip++;

    uint8_t args_combo = spu->execode[spu->ip];

    spu->ip++;


    int register_num = 0;

    if(args_combo & REGISTER_BIT)
    {
        memcpy(&register_num, spu->execode + spu->ip, sizeof(char));
        spu->ip++;
    }


    StackElem_t number = 0;

    if(args_combo & NUMBER_BIT)
    {
        memcpy(&number, spu->execode + spu->ip, sizeof(number));
        spu->ip += sizeof(StackElem_t);
    }


    StackElem_t result = 0;

    if(args_combo & SUMM_BIT)
    {
        result = number + spu->registers[register_num];
    }
    else if(args_combo & MULTIPL_BIT)
    {
        result = number * spu->registers[register_num];
    }
    else if(args_combo & REGISTER_BIT)
    {
        result = spu->registers[register_num];
    }
    else if(args_combo & NUMBER_BIT)
    {
        result = number;
    }
    else
    {
        fprintf(stderr, "WRONG ARGUMENT\n");
        return WRONG_ARGUMENT;
    }


    if(args_combo & RAM_BIT)
    {
        result = floor(result);

        if(result < -(EPSILON))
        {
            fprintf(stderr, "negative result\n");
            return NEG_RESULT;
        }


        if(result > RAM_COL * RAM_ROW)
        {
            fprintf(stderr, "RAM overflow\n");
            return RAM_OVERFLOW;
        }

        StackElem_t elem = spu->RAM[(size_t)result];
        Error_Codes stack_push_err = stack_push(&spu->stack, elem);

        if(stack_push_err != ALL_IS_OK)
        {
            return PUSH_ERROR;
        }

        return RIGHT_OPCODE;
    }
    else
    {
        Error_Codes stack_push_err = stack_push(&spu->stack, result);

        if(stack_push_err != ALL_IS_OK)
        {
            return PUSH_ERROR;
        }

        return RIGHT_OPCODE;
    }

    return RIGHT_OPCODE;
}


Opcode_Errors pop_maker(Struct_SPU* spu)
{
    if(spu->execode[spu->ip] != POP)
    {
        fprintf(stderr, "here's not a pop!\n");
        return WRONG_OPCODE;
    }

    spu->ip++;

    uint8_t args_combo = spu->execode[spu->ip];

    spu->ip++;


    int register_num = 0;

    if(args_combo & REGISTER_BIT)
    {
        memcpy(&register_num, spu->execode + spu->ip, sizeof(char));
        spu->ip++;
    }


    StackElem_t number = 0;

    if(args_combo & NUMBER_BIT)
    {
        memcpy(&number, spu->execode + spu->ip, sizeof(number));
        spu->ip += sizeof(StackElem_t);
    }

    StackElem_t result = 0;

    if(args_combo & RAM_BIT)
    {
        if(args_combo & SUMM_BIT)
        {
            StackElem_t reg_value = spu->registers[register_num];

            reg_value = floor(reg_value);

            if(reg_value > RAM_COL * RAM_ROW)
            {
                return RAM_OVERFLOW;
            }

            spu->RAM[(size_t)reg_value] = number;

            return RIGHT_OPCODE;
        }
        else if(args_combo & MULTIPL_BIT)
        {
            result = spu->registers[register_num];

            result += number;

            result = floor(result);

            if(result > RAM_COL * RAM_ROW)
            {
                return RAM_OVERFLOW;
            }

            StackElem_t elem = 0;

            Error_Codes stack_pop_err = stack_pop(&spu->stack, &elem);

            if(stack_pop_err != ALL_IS_OK)
                return POP_ERROR;

            spu->RAM[(size_t)result] = elem;

            return RIGHT_OPCODE;
        }
        else if(args_combo & REGISTER_BIT)
        {
            result = spu->registers[register_num];

            result = floor(result);

            if(result > RAM_COL * RAM_ROW)
                return RAM_OVERFLOW;

            spu->RAM[(size_t)result] = 1;

            return RIGHT_OPCODE;
        }
        else if(args_combo & NUMBER_BIT)
        {
            StackElem_t elem = 0;

            Error_Codes stack_pop_err = stack_pop(&spu->stack, &elem);

            if(stack_pop_err != ALL_IS_OK)
                return POP_ERROR;

            number = floor(number);

            if(number > RAM_COL * RAM_ROW)
                return RAM_OVERFLOW;

            spu->RAM[(size_t)number] = elem;

            return RIGHT_OPCODE;
        }
        else
        {
            fprintf(stderr, "RAM pop error\n");
            return WRONG_OPCODE;
        }
    }
    else
    {
        StackElem_t elem = 0;

        Error_Codes stack_pop_err = stack_pop(&spu->stack, &elem);

        if(stack_pop_err != ALL_IS_OK)
                return POP_ERROR;

        spu->registers[register_num] = elem;

        return RIGHT_OPCODE;
    }

    return RIGHT_OPCODE;
}


Error_Codes_SPU in_maker(Struct_SPU* SPU)
{
    StackElem_t elem_input = 0;
    printf("please, input a number: "); // FIXME add checks?
    scanf("%lg", &elem_input);
    Error_Codes stk_push_err = stack_push(&SPU->stack, elem_input);

    if(stk_push_err != ALL_IS_OK)
        return EXECUTION_ERR;

    SPU->ip++;

    return NORMAL_COND;
}


Error_Codes_SPU out_maker(Struct_SPU* SPU)
{
    StackElem_t elem = 0;
    Error_Codes_SPU getelem_err = getelem(SPU, &elem);

    if(getelem_err != NORMAL_COND)
        return EXECUTION_ERR;

    printf(BLUE_TEXT("outputed element ") GREEN_TEXT("%lg\n"), elem);

    SPU->ip++;

    return NORMAL_COND;
}


Error_Codes_SPU sqrt_maker(Struct_SPU* SPU)
{
    StackElem_t elem = 0;
    Error_Codes_SPU getelem_err = getelem(SPU, &elem);

    if(getelem_err != NORMAL_COND)
        return EXECUTION_ERR;

    elem = sqrt(elem);
    Error_Codes stk_push_err = stack_push(&SPU->stack, elem);

    if(stk_push_err != ALL_IS_OK)
        return EXECUTION_ERR;

    SPU->ip++;

    return NORMAL_COND;
}


Error_Codes_SPU sin_maker(Struct_SPU* SPU)
{
    StackElem_t elem = 0;
    Error_Codes_SPU getelem_err = getelem(SPU, &elem);

    if(getelem_err != NORMAL_COND)
        return EXECUTION_ERR;

    elem = sin(elem);
    Error_Codes stk_push_err = stack_push(&SPU->stack, elem);

    if(stk_push_err != ALL_IS_OK)
        return EXECUTION_ERR;

    SPU->ip++;

    return NORMAL_COND;
}


Error_Codes_SPU cos_maker(Struct_SPU* SPU)
{
    StackElem_t elem = 0;
    Error_Codes_SPU getelem_err = getelem(SPU, &elem);

    if(getelem_err != NORMAL_COND)
        return EXECUTION_ERR;

    elem = cos(elem);
    Error_Codes stk_push_err = stack_push(&SPU->stack, elem);

    if(stk_push_err != ALL_IS_OK)
        return EXECUTION_ERR;

    SPU->ip++;

    return NORMAL_COND;
}


void dump_maker(Struct_SPU* SPU)
{
    stack_dump(&SPU->stack, __FILE__, __func__, __LINE__);
    SPU->ip++;
}


Error_Codes_SPU call_maker(Struct_SPU* SPU)
{
    short label = 0;
    memcpy(&label, SPU->execode + SPU->ip + 1, sizeof(label));
    Error_Codes stack_push_err = helper_stack_push(&SPU->cllstk, SPU->ip + 3);

    if(stack_push_err != ALL_IS_OK)
        return EXECUTION_ERR;

    SPU->ip = (size_t)label;

    return NORMAL_COND;
}


Error_Codes_SPU ret_maker(Struct_SPU* SPU)
{
    size_t back_here = 0;
    // fprintf(stderr, "elem in call stack begin\n");
    Error_Codes stack_pop_err = helper_stack_pop(&SPU->cllstk, &back_here);

    if(stack_pop_err != ALL_IS_OK)
        return EXECUTION_ERR;
    // fprintf(stderr, "elem in call stack end\n");

    SPU->ip = back_here;

    return NORMAL_COND;
}

void jmp_maker(Struct_SPU* SPU)
{
    short label = 0;
    memcpy(&label, SPU->execode + SPU->ip + 1, sizeof(label));
    SPU->ip = (size_t)label;
}


void draw_maker(Struct_SPU* SPU)
{
    for(size_t ind_first = 0; ind_first < RAM_ROW; ind_first++)
    {
        for(size_t ind_second = 0; ind_second < RAM_COL; ind_second++)
        {
            if(fabs(SPU->RAM[ind_second + ind_first * RAM_COL]) < EPSILON )
            {
                fprintf(stderr, "* ");
            }
            else
            {
                 fprintf(stderr, "@ ");
            }
        }
        fprintf(stderr, "\n");
    }
    SPU->ip++;
}


void hlt_maker(Struct_SPU* SPU)
{
    SPU->flag = 0;
}


void dump_regs(const Struct_SPU spu, FILE* file)
{
    put_stars(file);

    fprintf(file, "\tDUMP REGISTERS\n\n");
    for(size_t i = 0; i < REGCOUNT; i++)
    {
        fprintf(file, "\tregister index:%03lu\n"
                      "\tregister value:%03lf", i, spu.registers[i]);

        if(spu.ip == i)
            fprintf(file, "<-- you're here now\n\n");
        else
            fprintf(file, "\n\n");
    }

    put_stars(file);
}


void dump_execode(const Struct_SPU spu, const size_t size, FILE* file)
{
    put_stars(file);
    fprintf(file, "\tDUMP EXECUTABLE CODE\n\n");
    put_stars(file);

    for(size_t i = 0; i < size; i++)
    {
        fprintf(file, "\tindex:%03lu    value:%03d", i, (int)spu.execode[i]);

        if(spu.ip == i)
            fprintf(file, "<-- you're here now\n");
        else
            fprintf(file, "\n");
    }

    put_stars(file);
    fprintf(file, "\n");
}


void dump_RAM(const Struct_SPU spu, FILE* file)
{
    put_stars(file);
    fprintf(file, "\tDUMP RAM\n\n");
    put_stars(file);

    for(size_t i = 0; i < RAM_SIZE; i++)
    {
        fprintf(file, "\tindex:%03lu    value:%03d", i, (int)spu.RAM[i]);

        if(spu.ip == i)
            fprintf(file, "<-- you're here now\n");
        else
            fprintf(file, "\n");
    }

    put_stars(file);
    fprintf(file, "\n");
}

void dump_spu(const Struct_SPU spu, FILE* file, const size_t execode_size)
{
    dump_execode(spu, execode_size, file);
    dump_regs(spu, file);
    dump_RAM(spu, file);
}


int compare_num(const StackElem_t number)
{
    return fabs(number) > EPSILON ? true : false;
}


Error_Codes_SPU put_arithm_res(Struct_SPU* SPU, const Arithmetic operation)
{

    StackElem_t first  = 0;
    StackElem_t second = 0;

    Error_Codes stk_pop_second_err = stack_pop(&SPU->stack, &second);

    if(stk_pop_second_err != ALL_IS_OK)
        return PUT_ARITHM_ERR;


    Error_Codes stk_pop_first_err  = stack_pop(&SPU->stack, &first);

    if(stk_pop_first_err != ALL_IS_OK)
        return PUT_ARITHM_ERR;


    StackElem_t result = 0;

    switch(operation)
    {
        case ADD_OP:
        {
            result = first + second;
            break;
        }
        case SUB_OP:
        {
            result = first - second;
            break;
        }
        case MUL_OP:
        {
            result = first * second;
            break;
        }
        case DIV_OP:
        {
            if(fabs(second) < EPSILON)
                return ZERO_DIVISION;

            result = first / second;
            break;
        }
        default:
        {
            assert(0 && "getres assert");
        };
    };

    SPU->ip++;

    Error_Codes stack_push_err = stack_push(&SPU->stack, result);

    if(stack_push_err != ALL_IS_OK)
        return PUT_ARITHM_ERR;

    return NORMAL_COND;
}


Error_Codes_SPU make_jump(Struct_SPU* SPU, const Compare_Operators operation)
{
    StackElem_t first = 0;
    StackElem_t second = 0;


    Error_Codes stk_pop_second_err = stack_pop(&SPU->stack, &second);

    if(stk_pop_second_err != ALL_IS_OK)
        return PUT_ARITHM_ERR;

    Error_Codes stk_pop_first_err  = stack_pop(&SPU->stack, &first);

    if(stk_pop_first_err != ALL_IS_OK)
        return PUT_ARITHM_ERR;


    int compare_result = 0;

    switch(operation)
    {
        case ABOVE:
        {
            compare_result = (first > second);
            break;
        }
        case ABOVE_EQUAL:
        {
            compare_result = (first > second || (fabs(first - second) < EPSILON));
            break;
        }
        case BELOW:
        {
            compare_result = (first < second);
            break;
        }
        case BELOW_EQUAL:
        {
            compare_result = (first < second || (fabs(first - second) < EPSILON));
            break;
        }
        case EQUAL:
        {
            compare_result = (fabs(first - second) < EPSILON);
            break;
        }
        case NOT_EQUAL:
        {
            compare_result = !(fabs(first - second) < EPSILON);
            break;
        }
        default:
        {
            assert(0 && "default assert in make jump\n");
        };
    };


    if(compare_result == 1)
    {
        short label = 0;
        memcpy(&label, SPU->execode + SPU->ip + 1, sizeof(label));
        SPU->ip = (size_t)label;
    }
    else
    {
        SPU->ip += 3;
    }

    return NORMAL_COND;
}


Error_Codes_SPU getelem(Struct_SPU* SPU, StackElem_t* elem)
{
    Error_Codes stk_pop_err = stack_pop(&SPU->stack, elem);

    if(stk_pop_err != ALL_IS_OK)
        return EXECUTION_ERR;

    return NORMAL_COND;
}