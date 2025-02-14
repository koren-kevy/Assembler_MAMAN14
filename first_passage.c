#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"
#include "Utility.h"

void add_label(Label_List **list, char *label_name, int address)
{
    Label_List *new_label = my_malloc(sizeof(Label_List));

    label_name[strcspn(label_name, "\n\r\t ")] = 0;
    memset(new_label->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_label->label, label_name);

    new_label->address = address;
    new_label->next = *list;
    *list = new_label;
}


void add_entry(Entry_List **list, char *line, int line_count)
{
    Entry_List *new_entry = my_malloc(sizeof(Entry_List));
    char *entry;

    entry = get_label(line, '\n',line_count);
    if(entry == NULL)
        return;

    memset(new_entry->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_entry->label, entry);

    free(entry);

    new_entry->next = *list;
    *list = new_entry;
    new_entry->add_list = NULL;
}


void add_extern(Extern_List **list, char *line, int line_count)
{
    Extern_List *new_extern = my_malloc(sizeof(Extern_List));
    char *exter;

    exter = get_label(line, '\n',line_count);
    if(exter == NULL)
        return;

    memset(new_extern->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_extern->label, exter);

    free(exter);

    new_extern->next = *list;
    *list = new_extern;
    new_extern->add_list = NULL;
}


char *get_label(char *line, char co ,int line_count)
{
    char *colon_ptr;
    char *label_name;
    int label_length;

    colon_ptr = strchr(line, co);
    if(colon_ptr == NULL)
    {
        printf("No colon found in label definition at line %d \n", line_count);
        return NULL;
    }

    label_length = (int)(colon_ptr - line);

    label_name = my_malloc(label_length + 1);
    strncpy(label_name, line, label_length);
    label_name[label_length] = '\0';

    return label_name;
}


int type_line_first_pass(char *line, char **command, int *length)
{
    int i = 0;
    if(strncmp(line, ".data", strlen(".data")) == 0)
    {
        *length = strlen(".data");
        return DATA;
    }
    if(strncmp(line, ".string", strlen(".string")) == 0)
    {
        *length = strlen(".string");
        return STRING;
    }
    if(strncmp(line, ".entry", strlen(".entry")) == 0)
    {
        *length = strlen(".entry");
        return ENTRY;
    }
    if(strncmp(line, ".extern", strlen(".extern")) == 0)
    {
        *length = strlen(".extern");
        return EXTERN;
    }

    for(; i < TOTAL_INSTRUCTIONS; i++)
    {
        if(strncmp(line, instruction_names[i], strlen(instruction_names[i])) == 0)
        {
            *length = strlen(instruction_names[i]);
            *command = instruction_names[i];
            return INSTRUCTION;
        }
    }
    return NONE;
}


Label_List *find_label(Label_List *list, char *label_name)
{
    while(list != NULL)
    {
        if(strcmp(list->label, label_name) == 0)
        {
            return list;
        }
        list = list->next;
    }
    return NULL;
}
