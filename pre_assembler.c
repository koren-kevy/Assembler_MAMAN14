#include "assembler_definitions.h"
#include "Utility.h"

void add_macro(Macro_List **list, char *macro_name)
{
    Macro_List *new_macro = my_malloc(sizeof(Macro_List));
    strcpy(new_macro->macro_name, macro_name);
    new_macro->head = NULL;
    new_macro->next = *list;
    *list = new_macro;
}

void add_macro_line(Macro_List *macro, char *line)
{
    Macro *new_line = my_malloc(sizeof(Macro));
    strcpy(new_line->line, line);
    new_line->next = macro->head;
    macro->head = new_line;
}

Macro_List *find_macro(Macro_List *list, char *macro_name)
{
    while(list)
    {
        if(strcmp(list->macro_name, macro_name) == 0)
            return list;
        list = list->next;
    }
    return NULL;
}

int type_line(char *line, Macro_List *list)
{
    char *cleaned_line = clean_line(line);
    if(strncmp(line, "mcro", strlen("mcro")) == 0) { return MACRO_DECLARATION; }
    if(strncmp(line, "mcroend", strlen("mcroend")) == 0) { return MACRO_END; }
    if(line[0] == '\0' || line[0] == '\n') { return EMPTY; }
    if(find_macro(list, cleaned_line) != NULL) { return SEEN_MACRO; }
    return NO_MACRO;
}

void pre_assembler(Assembler_Table **table_head, char *file_name)
{
    char *file_with_as, *file_with_am;
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LINE_LENGTH];
    FILE *fptr_as, *fptr_am;
    int line_type;
    int flag = TRUE;

    Macro *macro_head = NULL;
    Macro_List *list_head = NULL;

    memset(line, '\0', sizeof(line));
    memset(macro_name, '\0', sizeof(macro_name));

    /* To check file name */

    file_with_as = edit_file_name(file_name, ".as");
    file_with_am = edit_file_name(file_name, ".am");

    fptr_as = fopen(file_with_as, "r");
    if(fptr_as == NULL)
    {
        printf(" "); /* error */
        return;
    }

    fptr_am = fopen(file_with_am, "w");
    if(file_with_am == NULL)
    {
        printf(" "); /* error */
        return;
    }

    while(fgets(line, sizeof(line), fptr_as) != NULL)
    {
        line_type = type_line(line, (*table_head)->macro_head);

        switch(line_type)
        {
            case MACRO_DECLARATION:
                memset(line, '\0', sizeof(line));
                while(fgets(line, sizeof(line), fptr_as) != NULL && line_type != MACRO_END)
                {
                    line_type = type_line(line, ((*table_head)->macro_head));
                    if(line_type == MACRO_END)
                    {
                        /* Check for errors */
                        break;
                    }
                    if(flag == TRUE)
                    {
                        add_macro_line(&macro_head, clean_line(line));
                    }
                    memset(line, '\0', sizeof(line));
                }
                    if(flag == TRUE)
                    {
                        add_macro(&(*table_head)->macro_head, macro_name);
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
                fprintf(fptr_am, "%s", clean_line(line));
                break;
        }

        memset(line, '\0', sizeof(line));
        memset(macro_name, '\0', sizeof(macro_name));
    }

    free(file_with_as);
    file_with_as = NULL;
    fprintf(fptr_am, "%s", "\n");

    fclose(fptr_as);
    fclose(fptr_am);
}
