#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "../processor_headers/processor.h"
#include "../processor_headers/instructions.h"

#include "../../../stack_ded/src/stack_headers/stack.h"
#include "../../../stack_ded/src/stack_headers/colorised_output.h"
#include "../../../lib_buffer_proc/buffer.h"
#include "../../../lib_file_proc/file.h"


Error_Codes_SPU instructions(Struct_SPU* SPU, size_t buffer_size) // WTF
{
    while(SPU->flag && SPU->ip < buffer_size) // FIXME func!!!!!!
    {
        switch(SPU->execode[SPU->ip])
        {
            case(PUSH):
            {
               // fprintf(stderr, "case push\n");
                push_maker(SPU);
                // dump_spu(SPU, SPU->stack->dump_file, buffer_size);
                break;
            }
            case ADD:
            {
               // fprintf(stderr, "case add\n");
                put_arithm_res(SPU, ADD_OP);
                break;
            }
            case SUB:
            {
               // fprintf(stderr, "case sub\n");
                put_arithm_res(SPU, SUB_OP);
                break;
            }
            case MUL:
            {
               // fprintf(stderr, "case mul\n");
                put_arithm_res(SPU, MUL_OP);
                break;
            }
            case DIV:
            {
               // fprintf(stderr, "case div\n");
                put_arithm_res(SPU, DIV_OP);
                break;
            }
            case IN:
            {
               // fprintf(stderr, "case in\n");
                in_maker(SPU);
                break;
            }
            case OUT:
            {
               // fprintf(stderr, "case out\n");
                out_maker(SPU);
                break;
            }
            case SQRT:
            {
               // fprintf(stderr, "case sqrt\n");
                sqrt_maker(SPU);
                break;
            }
            case SIN:
            {
               // fprintf(stderr, "case sin\n");
                sin_maker(SPU);
                break;
            }
            case COS:
            {
               // fprintf(stderr, "case cos\n");
                cos_maker(SPU);
                break;
            }
            case DUMP:
            {
               // fprintf(stderr, "case dump\n");
                dump_maker(SPU);
                break;
            }
            case CALL:
            {
               // fprintf(stderr, "case call\n");
                call_maker(SPU);
                break;
            }
            case RET:
            {
               // fprintf(stderr, "case ret\n");
                ret_maker(SPU);
                break;
            }
            case JMP:
            {
               // fprintf(stderr, "case jmp\n");
                jmp_maker(SPU);
                break;
            }
            case JA:
            {
               // fprintf(stderr, "case ja\n");
                make_jump(SPU, ABOVE);
                break;
            }
            case JAE:
            {
               // fprintf(stderr, "case jae\n");
                make_jump(SPU, ABOVE_EQUAL);
                break;
            }
            case JB:
            {
               // fprintf(stderr, "case jb\n");
                make_jump(SPU, BELOW);
                break;
            }
            case JBE:
            {
               // fprintf(stderr, "case jbe\n");
                make_jump(SPU, BELOW_EQUAL);
                break;
            }
            case JE:
            {
               // fprintf(stderr, "case je\n");
                make_jump(SPU, EQUAL);
                break;
            }
            case JNE:
            {
               // fprintf(stderr, "case jne\n");
                make_jump(SPU, NOT_EQUAL);
                break;
            }
            case POP:
            {
                // fprintf(stderr, "case pop\n");
                // dump_spu(*SPU, SPU->stack.dump_file, buffer_size);
                pop_maker(SPU);
                break;
            }
            case DRAW:
            {
                draw_maker(SPU);
                fprintf(stderr, "\n\n");
                break;
            }
            case HLT:
            {
               // fprintf(stderr, "case hlt\n");
                hlt_maker(SPU);
                break;
            }
            default:
            {
                fprintf(stderr, "errm:%lu\n", (uint64_t)SPU->execode[SPU->ip]);
                assert(0 && "not good assert in instructions");
            };
        }
    }
    return NORMAL_COND;
}


Error_Codes_SPU run_code(const char* file_name)
{
    Struct_SPU SPU = {};

    FILE* file_read = nullptr;
    Files_Errors f_read_open_err = file_read_open(&file_read, file_name);

    if(f_read_open_err != ALL_GOOD_RET_F)
        return RUN_ERROR;


    size_t buffer_size = allocation_size(file_read);

    Error_Codes_SPU ctor_err = ctor_proc(&SPU, buffer_size, file_read);

    if(ctor_err != NORMAL_COND)
    {
        dtor_proc(&SPU);
        return EXECUTION_ERR;
    }

    Error_Codes_SPU instr_err = instructions(&SPU, buffer_size);

    if(instr_err != NORMAL_COND)
        return EXECUTION_ERR;

    Error_Codes_SPU dtor_err = dtor_proc(&SPU);

    if(dtor_err != NORMAL_COND)
        return EXECUTION_ERR;

    return NORMAL_COND;
}