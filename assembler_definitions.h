#ifndef ASSEMBLER_DEFINITIONS_H
#define ASSEMBLER_DEFINITIONS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MAX_LINE_LENGTH 81 /* Max line length is 80, we add 1 for '\n' */

#define MAX_LABEL_SIZE 32 /* Max label size is 31, we add 1 for ':' */

extern const char *instruction_names[];
extern const char *register_names[];

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

typedef enum
{
    r0 = 0,
    r1 = 1,
    r2 = 2,
    r3 = 3,
    r4 = 4,
    r5 = 5,
    r6 = 6,
    r7 = 7
} Register;

typedef enum
{
    MACRO_DECLARATION,
    MACRO_END,
    EMPTY,
    SEEN_MACRO,
    NO_MACRO
} Type_Line;

typedef struct
{
    unsigned int zero_flag : 1;
    unsigned int negetive_flag : 1;
    unsigned int carry_flag : 1;
    unsigned int overflow_flag : 1;
} psw_register;

typedef struct Word
{
    unsigned short word; /* Binary code for a word */
} Word;

typedef struct Macro
{
    char line[MAX_LINE_LENGTH]; /* Line of the macro */
    struct Macro *next; /*  A pointer to the next line */
} Macro;

typedef struct Macro_List
{
    char macro_name[MAX_LINE_LENGTH]; /* Name of the macro */
    Macro *head; /* Head of the macro */
    struct Macro_List *next; /* Pointer to the next macro */
} Macro_List;

typedef struct Label_List
{
    char label[MAX_LABEL_SIZE]; /* Label name */
    int address; /* Label address */
    struct Label_List *next; /* Pointer to the next label */
} Label_List;

typedef struct Address_List
{
    int address;
    struct Address_List *next;
} Address_List;

typedef struct Entry_List
{
    char label[MAX_LABEL_SIZE];
    Address_List *add_list;
    struct Entry_List *next;
} Entry_List;

typedef struct Extern_List
{
    char label[MAX_LABEL_SIZE];
    Address_List *add_list;
    struct Extern_List *next;
} Extern_List;

typedef struct Machine_Code_Command
{
    int add;
    Word binary_code;
    char label[MAX_LABEL_SIZE];
    int type;
    struct Machine_Code_Command *next;
} Machine_Code_Command;

typedef struct Machine_Code_Instructions
{
    int add;
    Word word;
    struct Machine_Code_Instructions *next;
} Machine_Code_Instructions;

typedef struct Assembler_Table
{
    Macro_List *macro_head;
    Label_List *label_head;
    Entry_List *entry_head;
    Extern_List *extern_head;
    Machine_Code_Command *command_head;
    Machine_Code_Instructions instructions_head;
} Assembler_Table;

#endif
