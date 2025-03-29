#ifndef UTILITY_H
#define UTILITY_H

#include "assembler_definitions.h"

void *my_malloc(long size);

char *clean_line(char *line);

char *edit_file_name(char *file_name, char *end);

void reverse_list_label(Label_List **head);

void reverse_list_entry(Entry_List **head);

void reverse_list_extern(Extern_List **head);

void reverse_list_command(Command_List **head);

void reverse_list_instruction(Instruction_List **head);

void reverse_list_address(Address_List **head);



void print_in_hex(unsigned int binary_code, int add, FILE *fptr);

void print_ob_file(Assembler_Table **tabel_head, char *file_name, int ic, int dc);

void print_ent_file(Entry_List *entry_list, char *file_name);

void print_ext_file(Extern_List *extern_list, char *file_name);



void free_macro_list(Macro_List *head);

void free_label_list(Label_List *head);

void free_entry_list(Entry_List *head);

void free_extern_list(Extern_List *head);

void free_command_list(Command_List *head);

void free_instruction_list(Instruction_List *head);

void free_address_list(Address_List *head);

void free_table(Assembler_Table *table);

#endif
