#include "assembler_definitions.h"
#include "Utility.h"


/**
 * This function allocates memory using the malloc function,
 * and check for memory allocation fails.
 * In case of a file, we exit the program.
 */
void* my_malloc(long size)
{
    void *memory = malloc(size); /* Allocating memory. */
    if(memory == NULL) /* In case of ane error we print it outn. */
    {
        printf("Memory allocation failed \n");
        exit(1); /* Exiting the program and freeing the memory. */
    }
    return memory;
}


/**
 * This line gets an assembly code line and cleans it from spaces,
 * while keeping the spaces within quotes. And returns the new line.
 */
char *clean_line(char *line)
{
    int in_quotes = FALSE;
    char *new_line = my_malloc(MAX_LINE_LENGTH);
    char *current = line; /* The current char in line. */
    char *dst = new_line; /* The destanation in which we put the next char in new_line. */

    while(*current)
        {
        if(*current == '"') /* If we found quotes we keep them as they are. */
        {
            in_quotes = !in_quotes;
            *dst++ = *current++;
        }
        else if(*current == ';') /* If we find ';' we remove it and what follows. */
        {
            *dst++ = '\n';
            current++;
        }
        else if(in_quotes || !isspace(*current)) /* In quotes, we keep the spaces. */
        {
            *dst++ = *current++;
        }
        else
        {
            current++;
        }
    }

    /* Add a '\n'. */
    if(*(dst - 1) != '\n')
    {
        *dst++ = '\n';
    }

    *dst = '\0';
    return new_line;
}


/**
 * This functions gets a file name an ending, and conects them together.
 */
char *edit_file_name(char *file_name, char *end)
{
    /* Initialize the new name. */
    char *new_name = my_malloc(MAX_LINE_LENGTH);
    memset(new_name, '\0', MAX_LINE_LENGTH);

    strcpy(new_name, file_name); /* Copy parameters into the new name. */
    strcat(new_name, end); /* Add to the new name the ending. */
    return new_name;
}


/**
 * This function reverses a labels list order.
 */
void reverse_list_label(Label_List **head)
{
    Label_List *current = *head;
    Label_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


/**
 * This function reverses an entries list order.
 */
void reverse_list_entry(Entry_List **head)
{
    Entry_List *current = *head;
    Entry_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


/**
 * This function reverses an externs list order.
 */
void reverse_list_extern(Extern_List **head)
{
    Extern_List *current = *head;
    Extern_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


/**
 * This function reverses a commands list order.
 */
void reverse_list_command(Command_List **head)
{
    Command_List *current = *head;
    Command_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


/**
 * This function reverses an instructions list order.
 */
void reverse_list_instruction(Instruction_List **head)
{
    Instruction_List *current = *head;
    Instruction_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}


/**
 * This function reverses an address list order.
 */
void reverse_list_address(Address_List **head)
{
    Address_List *current = *head;
    Address_List *prev = NULL, *next = NULL;

    /* We reverse the list by getting the next node and setting him to be the prev node. */
    while(current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

/**
 * This functions prints an address, and a binary code in hex form.
 */
void print_in_hex(unsigned int binary_code, int add, FILE *fptr)
{
    /* Printing the address 7 digits wide, and the binary code 6 digits wide. */
    fprintf(fptr, "%07d %06X\n", add, binary_code & MASK_24BITS);
}


/**
 * This function creates the ob file in the assembler transition.
 * It prints the values of ic and dc at the top,
 * Then prints addresses and words in hex.
 */
void print_ob_file(Assembly_Content **content_head, char *file_name, int ic, int dc)
{
    FILE *fptr = fopen(file_name, "w"); /* Opening the output file to write. */

    /* Initialize the lists. */
    Command_List *command_list = (*content_head)->command_head;
    Instruction_List *instruction_list = (*content_head)->instruction_head;

    if(fptr == NULL) /* Checking for an error opening the file. */
    {
        printf("Error opening file %s. \n", file_name);
        exit(1);
    }


    /* Printing the values of ic and dc. */
    fprintf(fptr, "%d %d \n", ic - 100, dc);


    /* Looping through the command list and printing the words. */
    while(command_list != NULL)
    {
        print_in_hex(command_list->binary_code.word, command_list->add, fptr);
        command_list = command_list->next;
    }

    /* Looping through the instruction list and printing the words. */
    while(instruction_list != NULL)
    {
        print_in_hex(instruction_list->word.word, instruction_list->add + ic - 1, fptr);
        instruction_list = instruction_list->next;
    }

    fclose(fptr);
}


/**
 * This function creates the ent file in the assembler transition.
 * It prints the entries defined in the program and their addresses.
 */
void print_ent_file(Entry_List *entry_list, char *file_name)
{
    FILE *fptr = fopen(file_name, "w"); /* Opening the output file to write. */
    Address_List *address_list;

    if(fptr == NULL) /* If there was an error opening the file. */
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

    while(entry_list != NULL) /* We loop through the entry list. */
    {
        /* We print each entry with its address. */
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


/**
 * This function creates the ext file in the assembler transition.
 * It prints the externs defined in the program and their addresses.
 */
void print_ext_file(Extern_List *extern_list, char *file_name)
{
    FILE *fptr = fopen(file_name, "w"); /* Opening the output file to write. */
    Address_List *address_list;

    if(fptr == NULL) /* If there was an error opening the file. */
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

    while(extern_list != NULL) /* We loop through the extern list. */
    {
        address_list = extern_list->add_list;
        while(address_list != NULL) /* We print each extern with its address. */
        {
            fprintf(fptr, "%s %d\n", extern_list->label, address_list->address);
            address_list = address_list->next;
        }
        extern_list = extern_list->next;
    }

    fclose(fptr);
}


/**
 * This function frees the memory allocated for the macro list.
 */
void free_macro_list(Macro_List *head)
{
    Macro_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/**
 * This function frees the memory allocated for the label list.
 */
void free_label_list(Label_List *head)
{
    Label_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/**
 * This function frees the memory allocated for the entry list.
 */
void free_entry_list(Entry_List *head)
{
    Entry_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;

        if(temp->add_list) /* For each node we also free its address list. */
        {
            free_address_list(temp->add_list);
            temp->add_list = NULL;
        }

        free(temp);
        temp = NULL;
    }
}


/**
 * This function frees the memory allocated for the extern list.
 */
void free_extern_list(Extern_List *head)
{
    Extern_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;

        if(temp->add_list) /* For each node we also free its address list. */
        {
            free_address_list(temp->add_list);
            temp->add_list = NULL;
        }

        free(temp);
        temp = NULL;
    }

}


/**
 * This function frees the memory allocated for the command list.
 */
void free_command_list(Command_List *head)
{
    Command_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/**
 * This function frees the memory allocated for the instruction list.
 */
void free_instruction_list(Instruction_List *head)
{
    Instruction_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/**
 * This function frees the memory allocated for the address list.
 */
void free_address_list(Address_List *head)
{
    Address_List *temp;
    while(head != NULL) /* Each time we free the head of the list and move to the next node. */
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


/**
 * This function calls the other functions and frees all the memory allocated
 * for the passes of the assembler. Then it frees itself.
 */
void free_content(Assembly_Content *content)
{
    if(content == NULL) return;

    /* Calling for each function to free its memory. */

    free_macro_list(content->macro_head);

    free_label_list(content->label_head);

    free_entry_list(content->entry_head);

    free_extern_list(content->extern_head);

    free_command_list(content->command_head);

    free_instruction_list(content->instruction_head);

    free(content);
}
