#include "assembler_definitions.h"
#include "Utility.h"
#include "error_handler.h"

/*
 * We will build functions that check errors,
 * if we get an error we will return an error value.
 * We will do it to every function, at the end we will
 * build a function that check if we got any error.
 */

char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea", "clr",
"not", "inc", "dec", "jmp", "bne", "jst", "red", "prn", "rts", "stop"};

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* This function already gets a line of a macro definition. */
int check_macro_name_for_instruction(char *macro_name, int count)
{
    int i = 0;
    for(; i < TOTAL_INSTRUCTIONS; i++)
    {
        if(strcmp(instruction_names[i], macro_name) == 0)
        {
            printf("Macro name is an instruction command in line %d. \n", count);
            return error;
        }
    }
    return no_error;
}

int check_macro_name_for_register(char *macro_name, int count)
{
    int i = 0;
    for(; i < TOTAL_REGISTERS; i++)
    {
        if(strcmp(register_names[i], macro_name) == 0)
        {
            printf("Macro name is a register name in line %d. \n", count);
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
                printf("In macro ending at line %d are extra charecters. \n", count);
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
