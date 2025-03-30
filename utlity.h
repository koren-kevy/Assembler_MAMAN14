#ifndef UTILITY_H
#define UTILITY_H

#include "assembler_definitions.h"


/**
 * This function allocates memory using the malloc function,
 * and check for memory allocation fails.
 * In case of a file, we exit the program.
 *
 * @param size The size of memory to allocate.
 * @return A pointer to the allocated memory.
 */
void *my_malloc(long size);


/**
 * This line gets an assembly code line and cleans it from spaces,
 * while keeping the spaces within quotes. And returns the new line.
 *
 * @param line A line of assembly code.
 * @return A line cleaned from spaces/tabs/..., but keeps the spaces within quotes.
 */
char *clean_line(char *line);


/**
 * This functions gets a file name an ending, and conects them together.
 *
 * @param file_name The file name.
 * @param end The file ending to add.
 * @return A string that consists of the file name and the ending together.
 */
char *edit_file_name(char *file_name, char *end);



/* I tried to use generic functions but because of different sizes for each type it wasn't possible. */
/* A reverse is necessary because everything was added to the head of the list. */


/**
 * This function reverses a labels list order.
 *
 * @param head A pointer to the head of the label list.
 */
void reverse_list_label(Label_List **head);


/**
 * This function reverses an entries list order.
 *
 * @param head A pointer to the head of the entry list.
 */
void reverse_list_entry(Entry_List **head);


/**
 * This function reverses an externs list order.
 *
 * @param head A pointer to the head of the extern list.
 */
void reverse_list_extern(Extern_List **head);


/**
 * This function reverses a commands list order.
 *
 * @param head A pointer to the head of a command list.
 */
void reverse_list_command(Command_List **head);


/**
 * This function reverses an instructions list order.
 *
 * @param head A pointer to the head of an instruction list.
 */
void reverse_list_instruction(Instruction_List **head);


/**
 * This function reverses an address list order.
 *
 * @param head A pointer to the head of an address list.
 */
void reverse_list_address(Address_List **head);


/**
 * This functions prints an address, and a binary code in hex form.
 *
 * @param binary_code The binary code to print as hex.
 * @param add The address to print.
 * @param fptr A pointer to a file to print into.
 */
void print_in_hex(unsigned int binary_code, int add, FILE *fptr);


/**
 * This function creates the ob file in the assembler transition.
 * It prints the values of ic and dc at the top,
 * Then prints addresses and words in hex.
 *
 * @param content A pointer to the head of the assembly content.
 * @param file_name The name of the file to write into.
 * @param ic The value of ic to print.
 * @param dc The value of dc to print.
 */
void print_ob_file(Assembly_Content **content, char *file_name, int ic, int dc);


/**
 * This function creates the ent file in the assembler transition.
 * It prints the entries defined in the program and their addresses.
 *
 * @param entry_list The entry list to print.
 * @param file_name The file name to print into.
 */
void print_ent_file(Entry_List *entry_list, char *file_name);


/**
 * This function creates the ext file in the assembler transition.
 * It prints the externs defined in the program and their addresses.
 *
 * @param extern_list The extern list to print.
 * @param file_name The file name to print into.
 */
void print_ext_file(Extern_List *extern_list, char *file_name);


/**
 * This function frees the memory allocated for the macro list.
 *
 * @param head The head of the macro list to free.
 */
void free_macro_list(Macro_List *head);


/**
 * This function frees the memory allocated for the label list.
 *
 * @param head The head of the label list.
 */
void free_label_list(Label_List *head);


/**
 * This function frees the memory allocated for the entry list.
 *
 * @param head The head of the entry list.
 */
void free_entry_list(Entry_List *head);


/**
 * This function frees the memory allocated for the extern list.
 *
 * @param head The head of the extern list.
 */
void free_extern_list(Extern_List *head);


/**
 * This function frees the memory allocated for the command list.
 *
 * @param head The head of the command list.
 */
void free_command_list(Command_List *head);


/**
 * This function frees the memory allocated for the instruction list.
 *
 * @param head The head of the instruction list.
 */
void free_instruction_list(Instruction_List *head);


/**
 * This function frees the memory allocated for the address list.
 *
 * @param head The head of the address list.
 */
void free_address_list(Address_List *head);


/**
 * This function calls the other functions and frees all the memory allocated
 * for the passes of the assembler. Then it frees itself.
 *
 * @param content The assembly content.
 */
void free_content(Assembly_Content *content);

#endif
