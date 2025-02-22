#ifndef PRE_AND_PASSAGES_H
#define PRE_AND_PASSAGES_H

#include "assembler_definitions.h"

#define PRE_PROC 100 /* A value to tell a function at which stage are we. */
#define FIRST_PASS 101

enum
{
    MACRO_DECLARATION,
    MACRO_END,
    EMPTY,
    SEEN_MACRO,
    NO_MACRO
};

void add_macro(Macro_List **list, char *macro_name, Macro *macro_head);

void add_macro_line(Macro **macro_head, char *line);

Macro_List *find_macro(Macro_List *list, char *macro_name);

int type_line(char *line, Macro_List *list);

void pre_assembler(Assembler_Table **table_head, char *file_name, char *file_with_as);


enum
{
    DATA,
    STRING,
    ENTRY,
    EXTERN,
    INSTRUCTION,
    NONE
};

void add_label(Label_List **list, char *label_name, int address);

void add_entry(Entry_List **list, char *line, int line_count);

void add_extern(Extern_List **list, char *line, int line_count);

void add_instruction(Machine_Code_Instructions **list, int address, Word word);

void add_command(Machine_Code_Command **list, int address, Word word, char label[MAX_LABEL_SIZE],
    int type);

void get_label(char *line, char *label, char co ,int line_count);

int get_string(Machine_Code_Instructions **list, char *line, int skip, int *address);

void get_data(Machine_Code_Instructions **list, char *line, int skip, int *address);

int get_operand_type(char *operand);

int get_operand_value(int type, char *operand);

int type_line_first_pass(char *line, Command *command, int *length);

Label_List *find_label(Label_List *list, char *label_name);

void make_word_for_one_operand_command(Command *command, Machine_Code_Command **list, int *ic,
    char *dest, int dest_type);

void make_word_for_two_operand_command(Command *command, Machine_Code_Command **list, int *ic,
    char *source, int source_type, char *dest, int dest_type);

int make_command(Machine_Code_Command **list, char *line, Command *command, int *ic, int count);

void first_passage(Assembler_Table **table_head, char *file_name);

#endif
