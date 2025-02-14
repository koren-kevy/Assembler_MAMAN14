#ifndef PRE_AND_PASSAGES_H
#define PRE_AND_PASSAGES_H

#include "assembler_definitions.h"


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
void pre_assembler(Assembler_Table **table_head, char *file_name);


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
char *get_label(char *line, char co ,int line_count);
int type_line_first_pass(char *line, char **command, int *length);
Label_List *find_label(Label_List *list, char *label_name);


#endif
