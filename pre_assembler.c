#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"
#include "Utility.h"

void print_all_macros(Macro_List *head)
{
    Macro_List *curr = head;
    printf("\n--- Defined Macros ---\n");
    while (curr != NULL)
    {
        printf("Macro name: %s\n", curr->macro_name);
        curr = curr->next;
    }
    printf("--- End of Macros ---\n");
}

void add_macro(Macro_List **list, char *macro_name, Macro *macro_head)
{
    /*
    Macro_List *node = NULL;
    Macro_List *new_macro = my_malloc(sizeof(Macro_List));
    memset(new_macro->macro_name, '\0', MAX_LINE_LENGTH);
    strcpy(new_macro->macro_name, macro_name);
    new_macro->head = macro_head;
    new_macro->next = NULL;

    if(*list == NULL)
    {
        *list = new_macro;
        return;
    }

    node = *list;
    while(node->next != NULL) { node = node->next; }
    node->next = new_macro; */

    Macro_List *new_macro = my_malloc(sizeof(Macro_List));

    macro_name[strcspn(macro_name, "\n\r\t ")] = '\0';
    memset(new_macro->macro_name, '\0', MAX_LINE_LENGTH);
    strcpy(new_macro->macro_name, macro_name);

    new_macro->head = macro_head;

    new_macro->next = *list;
    *list = new_macro;
}

void add_macro_line(Macro **macro_head, char *line)
{
    Macro *ptr = NULL;
    Macro *new_line = my_malloc(sizeof(Macro));

    memset(new_line->line, '\0', sizeof(new_line->line));
    strcpy(new_line->line, line);
    new_line->next = NULL;

    if(*macro_head == NULL)
    {
        *macro_head = new_line;
        return;
    }

    ptr = *macro_head;
    while(ptr->next != NULL) { ptr = ptr->next; }
    ptr->next = new_line;
}


Macro_List *find_macro(Macro_List *list, char *macro_name)
{
    macro_name[strcspn(macro_name, "\n\r\t ")] = '\0';
    while(list != NULL)
    {
        if(strcmp(list->macro_name, macro_name) == 0)
        {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

int type_line(char *line, Macro_List *list)
{
    char *cleaned_line = clean_line(line);
    char *macro_name = strtok(cleaned_line, " \t\n");
    cleaned_line[strcspn(cleaned_line, "\n\r\t ")] = '\0';
    if(strncmp(line, "mcroend", strlen("mcroend")) == 0) { return MACRO_END; }
    if(strncmp(line, "mcro", strlen("mcro")) == 0) { return MACRO_DECLARATION; }
    if(line[0] == '\0' || line[0] == '\n') { return EMPTY; }
    if(macro_name != NULL && find_macro(list, cleaned_line) != NULL) { return SEEN_MACRO; }
    return NO_MACRO;
}

/*
void expand_macro(Macro_List *macro, char *line, FILE *fptr_as)
{
    Macro_List *current = macro;
    while (current != NULL)
    {
        if (strcmp(line, current->macro_name) == 0)
            {
            Macro *macro_line = current->head;
            while (macro_line != NULL)
            {
                fprintf(fptr_as, "%s\n", macro_line->line);
                macro_line = macro_line->next;
            }
            return;
        }
        current = current->next;
    }
}
*/

void pre_assembler(Assembler_Table **table_head, char *file_name)
{
    char *file_with_as, *file_with_am;
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LINE_LENGTH];
    FILE *fptr_as, *fptr_am;
    int line_type, line_count, temp_count;
    int flag = TRUE, result;

    Macro *macro_head = NULL;
    Macro_List *list_head = NULL;

    line_count = 1;

    memset(line, '\0', sizeof(line));
    memset(macro_name, '\0', sizeof(macro_name));

    /* To check file name */

    file_with_as = edit_file_name(file_name, ".as");
    file_with_am = edit_file_name(file_name, ".am");


    fptr_as = fopen(file_with_as, "r");
    if(fptr_as == NULL)
    {
        printf("Error opening file %s \n", file_with_as); /* error */
        return;
    }

    fptr_am = fopen(file_with_am, "w");
    if(fptr_am == NULL)
    {
        printf("Error opening file %s \n", file_with_am); /* error */
        return;
    }

    while(fgets(line, sizeof(line), fptr_as) != NULL)
    {
        line_type = type_line(line, (*table_head)->macro_head);
        switch(line_type)
        {
            case MACRO_DECLARATION:
                result = sscanf(line + strlen("mcro"), "%s", macro_name);
                if(result == 1)
                {
                    result = check_macro_line(clean_line(line), line_count, macro_name);
                    flag = flag && result == no_error;
                }
                result = check_macro_name_for_instruction(macro_name, line_count) &&
                    check_macro_name_for_register(macro_name, line_count);
                flag = flag && result == no_error;
                temp_count = line_count;
                memset(line, '\0', sizeof(line));
                while(fgets(line, sizeof(line), fptr_as) != NULL)
                {
                    temp_count++;
                    line_type = type_line(line, (*table_head)->macro_head);
                    if(line_type == MACRO_END)
                    {
                        result = check_macro_line(line, temp_count, macro_name);
                        flag =  flag && result == no_error;
                        break;
                    }
                    if(flag == TRUE)
                    {
                        add_macro_line(&macro_head, line);
                    }
                    memset(line, '\0', sizeof(line));
                }
                    if(flag == TRUE)
                    {
                        add_macro(&(*table_head)->macro_head, macro_name, macro_head);
                    }
                break;

            case SEEN_MACRO:
                list_head = find_macro((*table_head)->macro_head, clean_line(line));
                if(list_head != NULL)
                {
                    macro_head = list_head->head;
                    while(macro_head != NULL)
                    {
                        fprintf(fptr_am, "%s", macro_head->line);
                        macro_head = macro_head->next;
                    }
                }
                break;

            case EMPTY:
                break;

            case NO_MACRO:
                fprintf(fptr_am, "%s", line);
                break;
        }

        memset(line, '\0', sizeof(line));
        memset(macro_name, '\0', sizeof(macro_name));
        line_count++;
    }

    free(file_with_as);
    file_with_as = NULL;
    fprintf(fptr_am, "%s", "\n");

    fclose(fptr_as);
    fclose(fptr_am);
}
