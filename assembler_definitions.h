#ifndef ASSEMBLER_DEFINITIONS_H
#define ASSEMBLER_DEFINITIONS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TRUE 1 /* A value to represent a true value. */
#define FALSE 0 /* A value to represent a false value. */


#define MAX_LINE_LENGTH 81 /* Max line length is 80, we add 1 for '\n'. */
#define MAX_LABEL_SIZE 32 /* Max label size is 31, we add 1 for ':'. */


/* The total amount of instructions and registers. */
#define TOTAL_INSTRUCTIONS 16
#define TOTAL_REGISTERS 8


#define MASK_24BITS 0xFFFFFF /* Masking words that go over 24 bits. */


/* The amount of bits needed for shifting to each position in a word. */
#define BITS_TO_MOVE_FOR_OPCODE 18
#define BITS_TO_MOVE_FOR_SOURCE_TYPE 16
#define BITS_TO_MOVE_FOR_DEST_TYPE 11
#define BITS_TO_MOVE_FOR_SOURCE_REGISTER 13
#define BITS_TO_MOVE_FOR_DEST_REGISTER 8
#define BITS_TO_MOVE_FOR_FUNCT 3


/* Values for A,R,E for the word. */
#define E 1
#define R 2
#define A 4


/* Arrays which hold instruction names, instrcution functs and register names. */
extern char *instruction_names[TOTAL_INSTRUCTIONS];
extern int instruction_functs[TOTAL_INSTRUCTIONS];
extern char *register_names[TOTAL_REGISTERS];


/* A value for each instruction. */
typedef enum
{
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 3,
    lea = 4,
    clr = 5,
    not = 6,
    inc = 7,
    dec = 8,
    jmp = 9,
    bne = 10,
    jsr = 11,
    red = 12,
    prn = 13,
    rts = 14,
    stop = 15
} Opcode;


/* A value to indicate an operand type. */
typedef enum
{
    NO_TYPE,
    REGISTER,
    IMMEDIATE,
    LABEL
} Operand_type;


/* Binary code for a word, we will use only 24 bits. */
typedef struct Word
{
    unsigned int word;
} Word;


/* A struct defining a macro. */
typedef struct Macro
{
    char line[MAX_LINE_LENGTH]; /* Line of the macro. */
    struct Macro *next; /*  A pointer to the next line. */
} Macro;


/* A struct defining a command. */
typedef struct
{
    char name[MAX_LINE_LENGTH]; /* The name of the command. */
    int code; /* The commands code. */
    int funct; /* The commands funct. */
} Command;


/* A struct defining a macro list. */
typedef struct Macro_List
{
    char macro_name[MAX_LINE_LENGTH]; /* The name of the macro. */
    Macro *head; /* Head of the macro list. */
    struct Macro_List *next; /* A pointer to the next macro. */
} Macro_List;


/* A struct defining a label list. */
typedef struct Label_List
{
    char label[MAX_LABEL_SIZE]; /* The name of the label. */
    char type[MAX_LABEL_SIZE]; /* The type of the label code/data/string/entry. */
    int address; /* Label address. */
    struct Label_List *next; /* A pointer to the next label. */
} Label_List;


/* A struct defining an address list. */
typedef struct Address_List
{
    int address; /* The address. */
    struct Address_List *next; /* A pointer to the next address. */
} Address_List;


/* A struct defining an entry list. */
typedef struct Entry_List
{
    char label[MAX_LABEL_SIZE]; /* The name of the entry. */
    Address_List *add_list; /* A pointer to an address list. */
    struct Entry_List *next; /* A pointer to the next entry. */
} Entry_List;


/* A struct defining an extern list. */
typedef struct Extern_List
{
    char label[MAX_LABEL_SIZE]; /* The name of the extern. */
    Address_List *add_list; /* A pointer to an address list. */
    struct Extern_List *next; /* A pointer to the next extern. */
} Extern_List;


/* A struct defining a command list. */
typedef struct Command_List
{
    int add; /* The address of the command. */
    Word binary_code; /* The binary code of the command. */
    char label[MAX_LABEL_SIZE]; /* The name of the label of the command. */
    int type; /* The type of the command. */
    struct Command_List *next; /* A pointer to the next command. */
} Command_List;


/* A struct defining an instruction list. */
typedef struct Instruction_List
{
    int add; /* The address of the instruction. */
    Word word; /* The binary code of the instruction. */
    struct Instruction_List *next; /* A pointer to the next instruction. */
} Instruction_List;


/* A struct defining all the lists needed for the assembler. */
typedef struct Assembly_Content
{
    Macro_List *macro_head; /* The head of the macro list. */
    Label_List *label_head; /* The head of the label list. */
    Entry_List *entry_head; /* The head of the entry list. */
    Extern_List *extern_head; /* The head of the extern list. */
    Command_List *command_head; /* The head of the command list. */
    Instruction_List *instruction_head; /* The head of the instruction list. */
} Assembly_Content;

#endif
