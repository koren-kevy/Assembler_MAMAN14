#include "assembler_definitions.h"
#include "Utility.h"
#include "error_handler.h"
#include "pre_and_passages.h"

/*
 * We will build functions that check errors,
 * if we get an error we will return an error value.
 * We will do it to every function, at the end we will
 * build a function that check if we got any error.
 */

char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea", "clr",
"not", "inc", "dec", "jmp", "bne", "jst", "red", "prn", "rts", "stop"};

int instruction_functs[] = {0, 0, 1, 2, 0, 1, 2, 3, 4, 1, 2, 3, 0, 0, 0, 0};

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


int check_space_and_colon(char *line, char *name, int count, int stage)
{
    char character = line[strlen(name)];
    if(character != ':' && stage == FIRST_PASS)
    {
        printf("After label definition in line %d, a colon must follow. \n", count);
        return error;
    }
    character = line[strlen(name+1)];
    if(character != ' ')
    {
        if(stage == PRE_PROC)
        {
            printf("After macro definition in line %d, a space most follow. \n", count);
        }
        else if(stage == FIRST_PASS)
        {
            printf("After label definition in line %d, a space most follow. \n", count);
        }
        return error;
    }
    return no_error;
}

/* This function already gets a line of a macro definition. */
int check_name_for_instruction(char *name, int count, int stage)
{
    int i = 0;
    for(; i < TOTAL_INSTRUCTIONS; i++)
    {
        if(strcmp(instruction_names[i], name) == 0)
        {
            if(stage == PRE_PROC)
            {
                printf("Macro name in line %d, is an instruction command. \n", count);
            }
            else if(stage == FIRST_PASS)
            {
                printf("Label name in line %d, is an instruction command. \n", count);
            }
            return error;
        }
    }
    return no_error;
}

int check_name_for_register(char *name, int count, int stage)
{
    int i = 0;
    for(; i < TOTAL_REGISTERS; i++)
    {
        if(strcmp(register_names[i], name) == 0)
        {
            if(stage == PRE_PROC)
            {
                printf("Macro name in line %d, is a register name. \n", count);
            }
            else if(stage == FIRST_PASS)
            {
                printf("Label name in line %d, is a register name. \n", count);
            }
            return error;
        }
    }
    return no_error;
}


/* Checks if in the macro start and end there is no more characters.
 * This function already gets a line of macro definition or ending. */
int check_macro_line(char *line, int count, char *macro_name)
{
    int i;
    char *rest_of_line;
    while(*line == ' ' || *line == '\t')
        line++;
    if(strncmp(line, "mcroend", strlen("mcroend")) == 0)
    {
        for(i = strlen("mcroend"); line[i] != '\0' && line[i] != '\n'; i++)
        {
            if(line[i] != ' ' || line[i] != '\t')
            {
                printf("In macro ending at line %d, are extra charecters. \n", count);
                return error;
            }
        }
    }
    else
    {
        line += strlen("mcro");
        while (*line == ' ' || *line == '\t')
            line++;

        rest_of_line = line + strlen(macro_name);

        while(*rest_of_line == ' ' || *rest_of_line == '\t')
            rest_of_line++;

        if(*rest_of_line != '\0' && *rest_of_line != '\n')
        {
            printf("In macro declaration at line %d, there are extra characters.\n", count);
            return error;
        }
    }
    return no_error;
}

int check_label_name(char *name, int count)
{
    if(!isalpha(*name))
    {
        printf("Label name in line %d, is not a valid label name. \n", count);
        return error;
    }
    return no_error;
}

int check_for_same_label(Label_List *list, char *name, int count)
{
    Label_List *result = find_label(list, name);
    if(result == NULL)
    {
        return no_error;
    }
    printf("Label name in line %d, is already a different label. \n", count);
    return error;
}

int check_data_line(char *line, int count, char *label)
{
    int expecting_num = TRUE;
    int is_num = FALSE;

    line += strlen(label);
    if(strncmp(line, ".data", strlen(".data")) != 0)
    {
        printf("In data instruction at line %d, there is no .data instruction. \n", count);
        return error;
    }
    while(*line != '\0')
    {
        if(expecting_num)
        {
            if(*line == '+' || *line == '-' || isdigit(*line))
            {
                is_num = TRUE;
                expecting_num = FALSE;
            }
            else
            {
                printf("In data instruction at line %d, there isn't a number defined. \n", count);
                return error;
            }
        }
        else
        {
            if(*line == ',')
            {
                expecting_num = TRUE;
            }
            else if(!isdigit(*line))
            {
                printf("In data instruction at line %d,"
                       " there is an undefined character. \n", count);
                return error;
            }
        }
        line++;
    }
    return !expecting_num && is_num;
}
