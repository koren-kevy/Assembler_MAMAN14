#ifndef PRE_AND_PASSAGES_H
#define PRE_AND_PASSAGES_H

#include "assembler_definitions.h"

void add_macro(Macro_List **list, char *macro_name, Macro *macro_head);
void add_macro_line(Macro **macro_head, char *line);
Macro_List *find_macro(Macro_List *list, char *macro_name);
int type_line(char *line, Macro_List *list);
void pre_assembler(Assembler_Table **table_head, char *file_name);


void add_label(Label_List **list, char *label_name, int address);
char *get_lable(char *line, int line_count);

#endif
