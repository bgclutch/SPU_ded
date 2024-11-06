#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "../../../stack_ded/src/stack_headers/stack.h"
#include "../../../spu_commands.h"


static const size_t RAM_ROW  = 32;
static const size_t RAM_COL  = 64;
static const size_t RAM_SIZE = RAM_ROW * RAM_COL;


enum Error_Codes_SPU
{
    NORMAL_COND    = 0x00,
    CTOR_ERR       = 0x01,
    DTOR_ERR       = 0x02,
    CODE_VER_ERR   = 0x03,
    RUN_ERROR      = 0x04,
    INSTR_ERROR    = 0x05,
    ZERO_DIVISION  = 0x06,
    PUT_ARITHM_ERR = 0x07,
    EXECUTION_ERR  = 0x08,
};

enum Opcode_Errors
{
    RIGHT_OPCODE   = 0x00,
    PUSH_ERROR     = 0x01,
    POP_ERROR      = 0x02,
    NEG_RESULT     = 0x03,
    RAM_OVERFLOW   = 0x04,
    WRONG_OPCODE   = 0x05,
    WRONG_ARGUMENT = 0x06,
};

enum Arithmetic
{
    ADD_OP = 0x01,
    SUB_OP = 0x02,
    MUL_OP = 0x03,
    DIV_OP = 0x04,
};

enum Compare_Operators
{
    ABOVE       = 0x01,
    ABOVE_EQUAL = 0x02,
    BELOW       = 0x03,
    BELOW_EQUAL = 0x04,
    EQUAL       = 0x05,
    NOT_EQUAL   = 0x06,
};

struct Struct_SPU
{
    uint8_t* execode;
    StackElem_t registers[REGCOUNT];
    Main_Stack_Struct stack;
    Main_Stack_Struct cllstk;
    size_t ip;
    StackElem_t RAM[RAM_SIZE];
    int flag;
};


Error_Codes_SPU ctor_proc(Struct_SPU *SPU, const size_t buffer_size, FILE* file);

Error_Codes_SPU dtor_proc(Struct_SPU *SPU);

Opcode_Errors push_maker(Struct_SPU* spu);

Opcode_Errors pop_maker(Struct_SPU* spu);


Error_Codes_SPU in_maker(  Struct_SPU* SPU);

Error_Codes_SPU out_maker( Struct_SPU* SPU);

Error_Codes_SPU sqrt_maker(Struct_SPU* SPU);

Error_Codes_SPU sin_maker( Struct_SPU* SPU);

Error_Codes_SPU cos_maker( Struct_SPU* SPU);

void dump_maker(Struct_SPU* SPU);

void jmp_maker( Struct_SPU* SPU);

Error_Codes_SPU call_maker(Struct_SPU* SPU);

Error_Codes_SPU ret_maker( Struct_SPU* SPU);

void draw_maker(Struct_SPU* SPU);

void hlt_maker( Struct_SPU* SPU);


Error_Codes_SPU put_arithm_res(Struct_SPU* SPU, const Arithmetic operation);

Error_Codes_SPU make_jump(Struct_SPU* SPU, const Compare_Operators operation);

Error_Codes_SPU getelem(Struct_SPU* SPU, StackElem_t* elem);

void dump_regs(Struct_SPU spu, FILE* file);

void dump_execode(Struct_SPU spu, size_t size, FILE* file);

void dump_RAM(Struct_SPU spu, FILE* file);

void dump_spu(Struct_SPU spu, FILE* file, size_t execode_size);

int compare_num(StackElem_t number);



#endif // PROCESSOR_H_