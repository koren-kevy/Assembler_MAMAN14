#ifndef ASSEMBLER_DEFINITIONS_H
#define ASSEMBLER_DEFINITIONS_H

typedef enum
{
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 2,
    lea = 4,
    clr = 5,
    not = 5,
    inc = 5,
    dec = 5,
    jmp = 9,
    bne = 9,
    jsr = 9,
    red = 12,
    prn = 13,
    rts = 14,
    stop = 15
} Opcode;

typedef enum
{
    funct_none = 0,
    funct_add = 1,
    funct_sub = 2,
    funct_clr = 1,
    funct_not = 2,
    funct_inc = 3,
    funct_dec = 4,
    funct_jmp = 1,
    funct_bne = 2,
    funct_jsr = 3
} Funct;

#endif
