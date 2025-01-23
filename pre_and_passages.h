#ifndef PRE_AND_PASSAGES_H
#define PRE_AND_PASSAGES_H

#include "assembler_definitions.h"

void add_macro(Macro_List **list, char *macro_name);
void add_macro_line(Macro_List *macro, char *line);
Macro_List *find_macro(Macro_List *list, char *macro_name);
int type_line(char *line, Macro_List *list);
void pre_assembler(Assembler_Table **table_head, char *file_name);

#endif
