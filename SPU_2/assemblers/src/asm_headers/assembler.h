#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_


#define ERROR_PROC(result) err_checker(result, __FILE__, __LINE__)
#define RET_ERR(result) res_checker(result, __FILE__, __LINE__)


const char *const c_push  = "push";
const char *const c_add   = "add";
const char *const c_sub   = "sub";
const char *const c_mul   = "mul";
const char *const c_div   = "div";
const char *const c_in    = "in";
const char *const c_out   = "out";
const char *const c_sqrt  = "sqrt";
const char *const c_sin   = "sin";
const char *const c_cos   = "cos";
const char *const c_dump  = "dump";
const char *const c_hlt   = "hlt";
const char *const c_pop   = "pop";
const char *const c_jmp   = "jmp";
const char *const c_ja    = "ja";
const char *const c_jae   = "jae";
const char *const c_jb    = "jb";
const char *const c_jbe   = "jbe";
const char *const c_je    = "je";
const char *const c_jne   = "jne";
const char *const c_call  = "call";
const char *const c_draw  = "draw";
const char *const c_ret   = "ret";
const char *const c_ax    = "AX";
const char *const c_bx    = "BX";
const char *const c_cx    = "CX";
const char *const c_dx    = "DX";


const char c_lblatr   = ':';
const char c_ramatrb  = '[';
const char c_ramatre  = ']';

const char *const c_add_op = "+";
const char *const c_mul_op = "*";

const char *const  c_basic_name = "aboba";
const size_t c_b_name_len = sizeof(c_basic_name);

const size_t  c_label_num = 10;
const ssize_t c_label_ip_null = -1;
const size_t  c_token_alloc_coef = 8;
const size_t  c_realloc_up_coef = 2;
const size_t  c_realloc_down_coef = 4;


#include "../../../stack_ded/src/stack_headers/stack.h"
#include "../../../spu_commands.h"


enum Asm_Errors
{
    ALL_OK_RETURN    = 0x00,
    SYNT_ERR         = 0x01,
    TOKEN_INIT_ERR   = 0x02,
    REALLOC_ERROR    = 0x03,
    TRANSLATOR_ERR   = 0x04,
    LBL_TO_BUF_ERR   = 0x06,
    GET_BINCODE_ERR  = 0x07,
    BIN_TO_FILE_ERR  = 0x08,
    FREE_ALL_ERR     = 0x09,
    DYN_TKN_DTOR_ERR = 0x0A,
    DYN_TKN_CTOR_ERR = 0x0B,
    BUFFER_NULLPTR   = 0x0C,
    PUSH_ARG_ERR     = 0x0D,
    POP_ARG_ERR      = 0x0F,
    GET_LBL_POS_ERR  = 0x10,
    TOO_MANY_LBLS    = 0x20,
};


enum Cmp_Res
{
    WRONG_CMP = 0x00,
    RIGHT_CMP = 0x01,
};


enum Token_Type
{
    ERROR     = -1,
    OPCODE    =  1,
    NUMBER    =  2,
    LABEL_DEF =  3,
    LABEL_USE =  4,
    REGISTER  =  5,
    PFLAG     =  6,
    ARITHM    =  7,
};


enum Run_Flags
{
    FIRST_RUN  = 0x52,
    SECOND_RUN = 0x53,
};



struct Token
{
    const char* name;           // uses always // FIXME pointer to str
    size_t name_size;        // uses always
    size_t token_size;       // uses always
    Token_Type type;         // uses always
    Commands opcode;         // used if opcode
    Registers register_num;  // used if register
    ssize_t label_address;   // used if label
    StackElem_t number;      // used if number
    Arithm_Op operation;     // used if push or pop before

};


struct Dynamic_Token
{
    struct Token* token_array;
    size_t capacity;
    size_t size;
};


struct Label
{
    const char* name; // FIXME pointer to str
    size_t name_size;
    int64_t ip;
};




Token_Type get_token_type(const char* name, const size_t name_size);

Cmp_Res opcode_cmp(const char* name, const size_t name_size);

Cmp_Res number_cmp(const char* name, const size_t name_size);

Cmp_Res register_cmp(const char* name, const size_t name_size);

Asm_Errors ctor_dyn_token(Dynamic_Token* token_st);

Asm_Errors dtor_dyn_token(Dynamic_Token* token_st);

Asm_Errors realloc_maker_token(Dynamic_Token* token_st, size_t new_capacity);

size_t realloc_if_up_needed_token(Dynamic_Token token_st);

size_t realloc_if_down_needed_token(Dynamic_Token token_st);

Asm_Errors init_token_st(char* buffer, size_t file_size, Dynamic_Token* token_st);

Asm_Errors translator(Dynamic_Token* token_st, FILE* file_dump, Label* labels_array, const Run_Flags translator_flag,
                      size_t* instruction_pointer, FILE* file_write);

Asm_Errors put_new_label(Dynamic_Token* token_st, Label* labels_array, size_t instr_pointer, const size_t ind,
                         const size_t labels_arr_len);

Asm_Errors put_label_to_buffer(Dynamic_Token* token_st, Label* labels_array, const size_t ind,
                              const size_t labels_arr_len, char* buffer, size_t* prefix_sum);

Asm_Errors ctor_labels(Label* labels_array, size_t size);

void label_dump(Label* labels_array, size_t labels_num, FILE* file);

void token_dump(Dynamic_Token* token, FILE* file);

Asm_Errors token_ctor(Token* token);

Asm_Errors label_to_null(Label* labels_array, size_t index);

Commands translation_func(const char*, size_t);

Registers choose_register(const char* command_word, size_t wrd_size);

int word_toupper_comparer(const char* str_1, const char* str_2, size_t len_1);

Arithm_Op choose_arithm_operation(const char* name, const size_t name_size);

Cmp_Res arithm_operand_cmp(const char* name, const size_t name_size);

Cmp_Res push_pop_atr_cmp(const char* name);

int err_checker(const Asm_Errors error_code, const char* name, const size_t line);

Asm_Errors get_bincode(const char* first_filename, const char* second_filename);

Asm_Errors bin_code_put_to_file(const char* second_filename, Dynamic_Token* token, Label* labels_array);

Asm_Errors assesmler_process(char* buffer, const size_t file_size, const char* second_filename);

#endif // ASSEMBLER_H_