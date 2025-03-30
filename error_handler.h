#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

/* Values to indicate if an error has occured. */
#define no_error 1
#define error 0


/**
 * This function checks if after a macro declaration or a label name there is
 * a space and a colon accordingly.
 *
 * @param line The line of assembly code to analyze.
 * @param name The name of a label/macro to skip over.
 * @param count The line of the assembly code.
 * @param stage An indicator to which stage are we, pre proc/first pass.
 * @return A value of an error/no error.
 */
int check_space_and_colon(char *line, char *name, int count, int stage);


/**
 * This function checks if a macro or label has the same name as an instruction.
 *
 * @param name The name to check.
 * @param count The number of the line.
 * @param stage An indicator to which stage are we, pre proc/first pass.
 * @return A value of an error/no error.
 */
int check_name_for_instruction(char *name, int count, int stage);


/**
 * This function checks if a macro or a label has the same name as a register.
 *
 * @param name The name to check for.
 * @param count The number of the line.
 * @param stage n indicator to which stage are we, pre proc/first pass.
 * @return A value of an error/no error.
 */
int check_name_for_register(char *name, int count, int stage);


/**
 * This function checks if at a macro declaration or ending there are extra charecters.
 *
 * @param line The assembly code line to analyze.
 * @param count The number of the line.
 * @param macro_name The name of the macro to skip over.
 * @return A value of an error/no error.
 */
int check_macro_line(char *line, int count, char *macro_name);


/**
 * This function checks the name of a macro/label to be legal or not.
 * A name must start with a letter and not a number or a different charaeter.
 *
 * @param name The name of a macro/label.
 * @param count The number of the line.
 * @param stage An indicator to which stage are we, pre proc/first pass.
 * @return A value of an error/no error.
 */
int check_legal_name(char *name, int count, int stage);


/**
 * This function checks if a label has already been definied and is defined again.
 *
 * @param list The label list to look in.
 * @param name The name of the label to look for.
 * @param count The number of the line.
 * @return A value of an error/no error.
 */
int check_for_same_label(Label_List *list, char *name, int count);


/**
 * This function gets a line of assembly code in which a ".data" is in.
 * The function checks for right declaration, right number of ',' and numbers.
 *
 * @param line The line of assembly code to analyze.
 * @param count The number of the line.
 * @param label The name of the label.
 * @return A value of an error/no error.
 */
int check_data_line(char *line, int count, char *label);

#endif
