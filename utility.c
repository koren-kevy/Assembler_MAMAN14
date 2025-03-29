#include "assembler_definitions.h"
#include "Utility.h"

void* my_malloc(long size)
{
    void *memory = malloc(size);
    if(memory == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return memory;
}

char *clean_line(char *line)
{
    int in_quotes = FALSE;
    char *new_line = my_malloc(MAX_LINE_LENGTH);
    char *current = line; /* The current char in line. */
    char *dst = new_line; /* The destanation in which we put the next char in new_line. */

    while(*current)
        {
        if(*current == '"')
        {
            in_quotes = !in_quotes;
            *dst++ = *current++;
        }
        else if(*current == ';')
        {
            *dst++ = '\n';
            current++;
        }
        else if(in_quotes || !isspace(*current))
        {
            *dst++ = *current++;
        }
        else
        {
            current++;
        }
    }

    if(*(dst - 1) != '\n')
    {
        *dst++ = '\n';
    }

    *dst = '\0';
    return new_line;
}

char *edit_file_name(char *file_name, char *end)
{
    char *new_name = my_malloc(MAX_LINE_LENGTH);
    memset(new_name, '\0', MAX_LINE_LENGTH);
    strcpy(new_name, file_name);
    strcat(new_name, end);
    return new_name;
}


void reverse_list_label(Label_List **head)
{
    Label_List *current = *head;
    Label_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_list_entry(Entry_List **head)
{
    Entry_List *current = *head;
    Entry_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_list_extern(Extern_List **head)
{
    Extern_List *current = *head;
    Extern_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_list_command(Command_List **head)
{
    Command_List *current = *head;
    Command_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_list_instruction(Instruction_List **head)
{
    Instruction_List *current = *head;
    Instruction_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

void reverse_list_address(Address_List **head)
{
    Address_List *current = *head;
    Address_List *prev = NULL, *next = NULL;

    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


void print_in_hex(unsigned int binary_code, int add, FILE *fptr)
{
    fprintf(fptr, "%07d %06X\n", add, binary_code);
}

void print_ob_file(Assembler_Table **tabel_head, char *file_name, int ic, int dc)
{
    FILE *fptr = fopen(file_name, "w");
    Command_List *command_list = (*tabel_head)->command_head;
    Instruction_List *instruction_list = (*tabel_head)->instruction_head;

    if(fptr == NULL)
    {
        printf("Error opening file %s. \n", file_name);
        exit(1);
    }

    fprintf(fptr, "%d %d \n", ic - 100, dc);

    while(command_list != NULL)
    {
        print_in_hex(command_list->binary_code.word, command_list->add, fptr);
        command_list = command_list->next;
    }

    while(instruction_list != NULL)
    {
        print_in_hex(instruction_list->word.word, instruction_list->add + ic - 1, fptr);
        instruction_list = instruction_list->next;
    }

    fclose(fptr);
}

void print_ent_file(Entry_List *entry_list, char *file_name)
{
    FILE *fptr = fopen(file_name, "w");
    Address_List *address_list;

    if(fptr == NULL)
    {
        printf("Error opening file %s. \n", file_name);
        exit(1);
    }

    /* Empty ent file, we dont print. */
    if(entry_list == NULL)
    {
        if(remove(file_name) != 0)
        {
            exit(1);
        }
        return;
    }

    while(entry_list != NULL)
    {
        address_list = entry_list->add_list;
        while(address_list != NULL)
        {
            fprintf(fptr, "%s %d\n", entry_list->label, address_list->address);
            address_list = address_list->next;
        }
        entry_list = entry_list->next;
    }

    fclose(fptr);
}

void print_ext_file(Extern_List *extern_list, char *file_name)
{
    FILE *fptr = fopen(file_name, "w");
    Address_List *address_list;

    if(fptr == NULL)
    {
        printf("Error opening file %s. \n", file_name);
        exit(1);
    }

    /* Empty ent file, we dont print. */
    if(extern_list == NULL)
    {
        if(remove(file_name) != 0)
        {
            exit(1);
        }
        return;
    }

    while(extern_list != NULL)
    {
        address_list = extern_list->add_list;
        while(address_list != NULL)
        {
            fprintf(fptr, "%s %d\n", extern_list->label, address_list->address);
            address_list = address_list->next;
        }
        extern_list = extern_list->next;
    }

    fclose(fptr);
}



void free_macro_list(Macro_List *head)
{
    Macro_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_label_list(Label_List *head)
{
    Label_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_entry_list(Entry_List *head)
{
    Entry_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;

        if(temp->add_list)
        {
            free_address_list(temp->add_list);
            temp->add_list = NULL;
        }

        free(temp);
        temp = NULL;
    }
}

void free_extern_list(Extern_List *head)
{
    Extern_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;

        if(temp->add_list)
        {
            free_address_list(temp->add_list);
            temp->add_list = NULL;
        }

        free(temp);
        temp = NULL;
    }

}

void free_command_list(Command_List *head)
{
    Command_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_instruction_list(Instruction_List *head)
{
    Instruction_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_address_list(Address_List *head)
{
    Address_List *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_table(Assembler_Table *table)
{
    if (table == NULL) return;

    free_macro_list(table->macro_head);

    free_label_list(table->label_head);

    free_entry_list(table->entry_head);

    free_extern_list(table->extern_head);

    free_command_list(table->command_head);

    free_instruction_list(table->instruction_head);

    free(table);
}
