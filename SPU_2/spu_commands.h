#ifndef SPU_COMMANDS_H_
#define SPU_COMMANDS_H_


enum Commands
{
    PUSH   = 0x01,
    ADD    = 0x02,
    SUB    = 0x03,
    MUL    = 0x04,
    DIV    = 0x05,
    IN     = 0x06,
    OUT    = 0x07,
    SQRT   = 0x08,
    SIN    = 0x09,
    COS    = 0x0A,
    DUMP   = 0x0B,
    JMP    = 0x10,
    JA     = 0x11,
    JAE    = 0x12,
    JB     = 0x13,
    JBE    = 0x14,
    JE     = 0x15,
    JNE    = 0x16,
    POP    = 0x20,
    CALL   = 0x30,
    RET    = 0x31,
    DRAW   = 0x52,
    HLT    = 0xEF,
    MATVEY = 0xFF,
};

enum Registers
{
    MY_AX    = 0x00,
    MY_BX    = 0x01,
    MY_CX    = 0x02,
    MY_DX    = 0x03,
    REGERR   = 0xDF,
    REGCOUNT = 0x04,
};


enum Arithm_Op
{
    NUMBER_BIT   = 0x01,
    REGISTER_BIT = 0x02,
    RAM_BIT      = 0x04,
    SUMM_BIT     = 0x08,
    MULTIPL_BIT  = 0x10,
    DEFAULT_BIT  = 0xFF,
};


#endif // SPU_COMAMNDS_H_