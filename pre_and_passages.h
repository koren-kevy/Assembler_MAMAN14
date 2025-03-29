#ifndef PRE_AND_PASSAGES_H
#define PRE_AND_PASSAGES_H

#include "assembler_definitions.h"

/* A few values to tell a function at which stage are we in. */
#define PRE_PROC 100
#define FIRST_PASS 101
#define SECOND_PASS 102


/* Pre proc declarations */
enum
{
    MACRO_DECLARATION,
    MACRO_END,
    EMPTY,
    SEEN_MACRO,
    NO_MACRO
};


/**
 * This function adds a macro into a macro list, allocates memory
 * for the new node and inserts it to the beginning of the list.
 *
 * @param list A pointer to the head of the macro list.
 * @param macro_name The name of the macro to add to the list.
 * @param macro_head The macro content to add to the list.
 */
void add_macro(Macro_List **list, char *macro_name, Macro *macro_head);


/**
 * This function adds a line into a macro content, allocates memory
 * and inserts it to the end of the content.
 *
 * @param macro_head A pointer to the content of a macro
 * @param line The line to add to the content
 */
void add_macro_line(Macro **macro_head, char *line);


/**
 * This function looks for a node in a macro list.
 *
 * @param list The macro list.
 * @param macro_name The name of the macro we are looking for.
 * @return The node which has the name we are looking for,
 * if he is not found, we return NULL.
 */
Macro_List *find_macro(Macro_List *list, char *macro_name);


/**
 * This function gets an assembly code line and finds if the line is
 * a macro declaration, macro ending, empty line, no macro, or a macro that was seen before.
 *
 * @param line The assembly line of code
 * @param list A macro list.
 * @return The type of the line:
 * Macro declaration/Macro end/Macro has been seen before/Empty line/No macro.
 */
int type_line(char *line, Macro_List *list);


/**
 * This function takes a file of assembly code, and expands the macros in a new file.
 * Then sends the new file into the next step of the assembler.
 *
 * @param table_head A pointer to an assembler table.
 * @param file_name The name of the file to be used later.
 * @param file_with_as The name of the file to execute.
 */
void pre_assembler(Assembler_Table **table_head, char *file_name, char *file_with_as);

/* End of pre proc declarations */


/* First pass declarations */

enum
{
    DATA,
    STRING,
    ENTRY,
    EXTERN,
    INSTRUCTION,
    NONE
};


/**
 * This function adds a label to the head of the label list.
 *
 * @param list A pointer to the head of the label list.
 * @param label_name The name of the label to add to the list.
 * @param address The address of the label to add to the list.
 * @param type The type of the label: data/string/code.
 */
void add_label(Label_List **list, char *label_name, int address, char *type);


/**
 * This function adds an entry to the head of the entry list.
 *
 * @param list A pointer to the head of the entry list.
 * @param entry The name of the entry label to add to the list.
 */
void add_entry(Entry_List **list, char *entry);


/**
 * This function adds an extern to the head of the extern list.
 *
 * @param list A pointer to the head of the extern list.
 * @param exter The name of the extern label to add to the list.
 */
void add_extern(Extern_List **list, char *exter);


/**
 * This function adds an instruction to the head of the instructions list.
 *
 * @param list A pointer to the head of the instructions list.
 * @param address The address in which to add to the instruction list.
 * @param word The binary code in which to add to the instruction list.
 */
void add_instruction(Instruction_List **list, int address, Word word);


/**
 * This function adds a command to the head of the commands list.
 *
 * @param list A pointer to the head of the commands list.
 * @param address The address of the new command to add to the list.
 * @param word The binary code of the new command to add to the list.
 * @param label The label name (if there is one) of the new command to add to the list.
 * @param type The type of the new command to add to the list.
 */
void add_command(Command_List **list, int address, Word word, char label[MAX_LABEL_SIZE],
                 int type);


/**
 * This functions extracts the name of a label from the line it's declared in.
 *
 * @param line A line of assembly code to handle.
 * @param label The labels name to handle.
 * @param co The char to look for and after, mostly ':'.
 * @param line_count The number of the line in the assembly file.
 */
void get_label(char *line, char *label, char co ,int line_count);


/**
 * This function extracts a string, adds the instruction, and updates the value of dc.
 *
 * @param list A pointer to the head of the instructions list.
 * @param line A line of assembly code to handle.
 * @param line_count The number of the line in the assembly file.
 * @param address The value of dc to update.
 */
void get_string(Instruction_List **list, char *line, int line_count, int *address);


/**
 * This function extracts a data type, adds the instruction, and updates the value of dc.
 *
 * @param list A pointer to the head of the instructions list.
 * @param line A line of assembly code to handle
 * @param skip The length of the labels name to skip on.
 * @param address The value of dc to update.
 */
void get_data(Instruction_List **list, char *line, int skip, int *address);


/**
 * This function gets an operand and analyzes it.
 * It checks if it's a label, register, a number or none of them.
 *
 * @param operand A part of an assembly line, an operand.
 * @return The type of the operand: register/immediate/label/no type
 */
int get_operand_type(char *operand);


/**
 * This function gets an operand and its type, and analyzes it.
 * It returns a value accurdenly.
 *
 * @param type The type of the operand.
 * @param operand The operand.
 * @return The value of an operand: register value/number value/label value.
 */
int get_operand_value(int type, char *operand);


/**
 * This function gets an assembly code and determines what happends in the line,
 * in order to know how to handle it.
 *
 * @param line A line of assembly code.
 * @param command A command to update in case of an instruction.
 * @param length The length to update accurding to the instruction/definition.
 * @return The type of the assembly line: data/entry/extern/instruction/none of them.
 */
int type_line_first_pass(char *line, Command *command, int *length);


/**
 *
 * @param command A command to analyze.
 * @param list A pointer to a command list.
 * @param ic The value of ic.
 * @param dest The destination operand.
 * @param dest_type The type of the destination operand.
 */
void make_word_for_one_operand_command(Command *command, Command_List **list, int *ic,
                                       char *dest, int dest_type);

void make_word_for_two_operand_command(Command *command, Command_List **list, int *ic,
    char *source, int source_type, char *dest, int dest_type);

int make_command(Command_List **list, char *line, Command *command, int *ic, int count);

void first_passage(Assembler_Table **table_head, char *og_name, char *file_name);

/* End of first pass */


/* Second pass */

void add_address(Address_List **list, int address);

Label_List *find_label(Label_List *list, char *label_name);

Entry_List *find_entry(Entry_List *list, char *entry_name);

Extern_List *find_extern(Extern_List *list, char *extern_name);

void second_pass(Assembler_Table **table, char *filename, int ic, int dc, int total_error);

/* End of second pass */

#endif
