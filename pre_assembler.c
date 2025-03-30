#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"
#include "Utility.h"


/**
 * This function adds a macro into a macro list, allocates memory
 * for the new node and inserts it to the beginning of the list.
 */
void add_macro(Macro_List **list, char *macro_name, Macro *macro_head)
{
    Macro_List *new_macro = my_malloc(sizeof(Macro_List));

    /* Remove whitespace from macro name. */
    macro_name[strcspn(macro_name, "\n\r\t ")] = '\0';

    /* Initialize macro name field. */
    memset(new_macro->macro_name, '\0', MAX_LINE_LENGTH);
    strcpy(new_macro->macro_name, macro_name);

    /* Assign macro content and insert into the head of the list. */
    new_macro->head = macro_head;
    new_macro->next = *list;
    *list = new_macro;
}

/**
 * This function adds a line into a macro content, allocates memory
 * and inserts it to the end of the content.
 */
void add_macro_line(Macro **macro_head, char *line)
{
    Macro *ptr = NULL;
    Macro *new_line = my_malloc(sizeof(Macro));

    /* Initialize the new macro line. */
    memset(new_line->line, '\0', sizeof(new_line->line));
    strcpy(new_line->line, line);
    new_line->next = NULL;

    /* If macro list is empty, assign the new line as the head. */
    if(*macro_head == NULL)
    {
        *macro_head = new_line;
        return;
    }

    /* Traverse to the last line and append new line. */
    ptr = *macro_head;
    while(ptr->next != NULL) { ptr = ptr->next; }
    ptr->next = new_line;
}

/**
 * This function looks for a node in a macro list.
 */
Macro_List *find_macro(Macro_List *list, char *macro_name)
{
    /* Remove whitespace from macro name. */
    macro_name[strcspn(macro_name, "\n\r\t ")] = '\0';
    while(list != NULL)
    {
        /* Search for the same name. */
        if(strcmp(list->macro_name, macro_name) == 0)
        {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

/**
 * This function gets an assembly code line and finds if the line is
 * a macro declaration, macro ending, empty line, no macro, or a macro that was seen before.
 */
int type_line(char *line, Macro_List *list)
{
    char *cleaned_line = clean_line(line);

    /* Clean the macro name. */
    char *macro_name = strtok(cleaned_line, " \t\n");

    /* Identify line type. */
    cleaned_line[strcspn(cleaned_line, "\n\r\t ")] = '\0';
    if(strncmp(line, "mcroend", strlen("mcroend")) == 0) { return MACRO_END; }
    if(strncmp(line, "mcro", strlen("mcro")) == 0) { return MACRO_DECLARATION; }
    if(line[0] == '\0' || line[0] == '\n') { return EMPTY; }
    if(macro_name != NULL && find_macro(list, cleaned_line) != NULL) { return SEEN_MACRO; }
    return NO_MACRO;
}

/**
 * This function takes a file of assembly code, and expands the macros in a new file.
 * Then sends the new file into the next step of the assembler.
 * Meanwhile, the function also calls for error checking functions.
 */
void pre_assembler(Assembly_Content **content_head, char *file_name, char *file_with_as)
{
    char *file_with_am;
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_LINE_LENGTH];
    FILE *fptr_as, *fptr_am;
    int line_type, line_count, temp_count;
    int flag = TRUE, result;

    Macro *macro_head = NULL;
    Macro_List *list_head = NULL;

    line_count = 1;

    /* Initialize line and macro name. */
    memset(line, '\0', sizeof(line));
    memset(macro_name, '\0', sizeof(macro_name));

    /* Create output file name. */
    file_with_am = edit_file_name(file_name, ".am");

    /* Open input file. */
    fptr_as = fopen(file_with_as, "r");
    if(fptr_as == NULL)
    {
        printf("Error opening file %s. \n", file_with_as); /* error */
        return;
    }

    /* Open output file. */
    fptr_am = fopen(file_with_am, "w");
    if(fptr_am == NULL)
    {
        printf("Error opening file %s. \n", file_with_am); /* error */
        return;
    }

    /* Read input file line by line. */
    while(fgets(line, sizeof(line), fptr_as) != NULL)
    {
        line_type = type_line(clean_line(line), (*content_head)->macro_head);
        switch(line_type)
        {
            case MACRO_DECLARATION:
                strcpy(macro_name, clean_line(line) + strlen("mcro"));

                /* Check macro declaration. */
                result = check_space_and_colon(line, macro_name, line_count, PRE_PROC);
                flag = flag && result == no_error;
                result = check_macro_line(line, line_count, macro_name);
                flag = flag && result == no_error;
                result = check_legal_name(macro_name, line_count, PRE_PROC);
                flag = flag && result == no_error;
                result = check_name_for_instruction(macro_name, line_count, PRE_PROC) &&
                    check_name_for_register(macro_name, line_count, PRE_PROC);
                flag = flag && result == no_error;

                /* Process macro content. */
                temp_count = line_count;
                memset(line, '\0', sizeof(line));
                while(fgets(line, sizeof(line), fptr_as) != NULL)
                {
                    temp_count++;
                    line_type = type_line(line, (*content_head)->macro_head);

                    /* Keep reading macro content untill "mcroend" is seen. */
                    if(line_type == MACRO_END)
                    {
                        result = check_macro_line(line, temp_count, macro_name);
                        flag =  flag && result == no_error;
                        break;
                    }
                    /* In case of no error in macro declaration we add him to the list. */
                    if(flag == TRUE)
                    {
                        add_macro_line(&macro_head, line);
                    }
                    memset(line, '\0', sizeof(line));
                }
                    if(flag == TRUE)
                    {
                        add_macro(&(*content_head)->macro_head, macro_name, macro_head);
                    }
                break;

            case SEEN_MACRO:
                /* A macro to expend. */
                list_head = find_macro((*content_head)->macro_head, clean_line(line));
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
                /* No macro just print into the file the same line. */
                fprintf(fptr_am, "%s", line);
                break;
        }

        /* Initialize line and macro name again. */
        memset(line, '\0', sizeof(line));
        memset(macro_name, '\0', sizeof(macro_name));
        line_count++;
    }

    free(file_with_as);
    file_with_as = NULL;

    fclose(fptr_as);
    fclose(fptr_am);

    /* In case of no error we move to the first pass, else we delete the file. */
    if(flag)
    {
        first_passage(content_head, file_name, file_with_am);
    }
    else
    {
        if(remove(file_with_am) != 0)
        {
            exit(1);
        }
    }
}
