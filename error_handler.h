#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define no_error 1
#define error 0

int check_space_and_colon(char *line, char *name, int count, int stage);

int check_name_for_instruction(char *name, int count, int stage);

int check_name_for_register(char *name, int count, int stage);

int check_macro_line(char *line, int count, char *macro_name);

int check_legal_name(char *name, int count, int stage);

int check_for_same_label(Label_List *list, char *name, int count);

int check_data_line(char *line, int count, char *label);

#endif
