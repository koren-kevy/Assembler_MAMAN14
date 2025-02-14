#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define no_error 1
#define error 0


int check_macro_name_for_instruction(char *macro_name, int count);
int check_macro_name_for_register(char *macro_name, int count);
int check_macro_line(char *line, int count, char *macro_name);

#endif
