#include "assembler_definitions.h"
#include "Utility.h"
#include "error_handler.h"

/*
 * We will build functions that check errors,
 * if we get an error we will return an error value.
 * We will do it to every function, at the end we will
 * build a function that check if we got any error.
 */


const char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea", "clr",
"not", "inc", "dec", "jmp", "bne", "jst", "red", "prn", "rts", "stop"};

/* This function already gets a line of a macro definition. */
int check_macro_name(char *line)
{
    int i = 0;
    char *new_line = clean_line(line);
    new_line += 4; /* To skip on the word "mcro" */
    for(; i < strlen(new_line); i++)
    {
        if(strcmp(instruction_names[i], new_line) == 0)
        {
            return error;
        }
    }
    return no_error;
}

/* Checks if in the macro start and end there is no more characters.
 * This function already gets a line of macro definition or ending. */
int check_macro_line(char *line)
{
    if(strcmp(line, "mcro") != 0 || strcmp(line, "mcroend") != 0)
    {
        return error;
    }
    return no_error;
}

int check_for_errors(char *line)
{
    if(check_macro_name(line) == error)
    {
        printf("\n"); /* error message */
        return FALSE;
    }
    if(check_macro_line(line) == error)
    {
        printf("\n");
        return FALSE;
    }

    return TRUE;
}
