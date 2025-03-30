#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"


char *instruction_names[] = {"mov", "cmp", "add", "sub", "lea", "clr",
"not", "inc", "dec", "jmp", "bne", "jst", "red", "prn", "rts", "stop"};

int instruction_functs[] = {0, 0, 1, 2, 0, 1, 2, 3, 4, 1, 2, 3, 0, 0, 0, 0};

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


/**
 * This function checks if after a macro declaration or a label name there is
 * a space and a colon accordingly.
 */
int check_space_and_colon(char *line, char *name, int count, int stage)
{
    char character = line[strlen(name)];

    /* If after the name in the first pass a colon didn't appear we print an error. */
    if(character != ':' && stage == FIRST_PASS)
    {
        printf("After label definition in line %d, a colon must follow. \n", count);
        return error;
    }

    /* If after a macro name a space didn't follow we print an error. */
    if(stage == PRE_PROC)
    {
        character = line[strlen("mcro")];
        if(character != ' ')
        {
            printf("After macro definition in line %d, a space most follow. \n", count);
            return error;
        }
    }
    else if(stage == FIRST_PASS) /* The same in the case of a label. */
    {
        character = line[strlen(name) + 1];
        if(character != ' ')
        {
            printf("After label definition in line %d, a space most follow. \n", count);
            return error;
        }
    }
    return no_error;
}


/**
 * This function checks if a macro or label has the same name as an instruction.
 */
int check_name_for_instruction(char *name, int count, int stage)
{
    int i = 0;

    /* We loop through every instruction and look for the same name. If found we print an error. */
    for(; i < TOTAL_INSTRUCTIONS; i++)
    {
        if(strcmp(instruction_names[i], name) == 0)
        {
            if(stage == PRE_PROC) /* If a macro has the same name as an instruction. */
            {
                printf("Macro name in line %d, is an instruction command. \n", count);
            }
            else if(stage == FIRST_PASS) /* If a label ahs the same name as an instruction. */
            {
                printf("Label name in line %d, is an instruction command. \n", count);
            }
            return error;
        }
    }
    return no_error;
}


/**
 * This function checks if a macro or a label has the same name as a register.
 */
int check_name_for_register(char *name, int count, int stage)
{
    int i = 0;
    /* We loop through every register and look for the same name. If found we print an error. */
    for(; i < TOTAL_REGISTERS; i++)
    {
        if(strcmp(register_names[i], name) == 0)
        {
            if(stage == PRE_PROC) /* If a macro has the same name as a register. */
            {
                printf("Macro name in line %d, is a register name. \n", count);
            }
            else if(stage == FIRST_PASS) /* If a label has the same name as a register. */
            {
                printf("Label name in line %d, is a register name. \n", count);
            }
            return error;
        }
    }
    return no_error;
}


/**
 * This function checks if at a macro declaration or ending there are extra charecters.
 */
int check_macro_line(char *line, int count, char *macro_name)
{
    int i;
    char *rest_of_line;
    while(*line == ' ' || *line == '\t')
        line++;
    if(strncmp(line, "mcroend", strlen("mcroend")) == 0) /* At macro ending line. */
    {
        /* If there is a character other than ' ' or tab we print an error. */
        for(i = strlen("mcroend") + 1; line[i] != '\0' && line[i] != '\n'; i++)
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
        /* If after macro declarations there are extra charecters we print an error. */
        line += strlen("mcro");
        while (*line == ' ' || *line == '\t')
            line++;

        rest_of_line = line + strlen(macro_name); /* Skip on the macro name. */

        while(*rest_of_line == ' ' || *rest_of_line == '\t')
            rest_of_line++;

        if(*rest_of_line != '\0' && *rest_of_line != '\n') /* A different charecter. */
        {
            printf("In macro declaration at line %d, there are extra characters.\n", count);
            return error;
        }
    }
    return no_error;
}


/**
 * This function checks the name of a macro/label to be legal or not.
 * A name must start with a letter and not a number or a different charaeter.
 */
int check_legal_name(char *name, int count, int stage)
{
    if(!isalpha(*name)) /* If the name doesn't start with a letter. */
    {
        if(stage == PRE_PROC) /* A macro name doesn't start with a letter, print error. */
        {
            printf("Macro name in line %d, is not a macro label name. \n", count);
            return error;
        }
        if(stage == FIRST_PASS) /* A label name doesn't start with a letter, print error. */
        {
            printf("Label name in line %d, is not a valid label name. \n", count);
            return error;
        }
    }
    return no_error;
}


/**
 * This function checks if a label has already been definied and is defined again.
 */
int check_for_same_label(Label_List *list, char *name, int count)
{
    Label_List *result = find_label(list, name); /* Looking for the label. */

    if(result == NULL || strcmp(result->type, ".entry") == 0) /* If not found there is no error. */
    {
        return no_error;
    }

    /* Error found, we print it. */
    printf("Label name in line %d, is already a different label. \n", count);
    return error;
}


/**
 * This function gets a line of assembly code in which a ".data" is in.
 * The function checks for right declaration, right number of ',' and numbers.
 */
int check_data_line(char *line, int count, char *label)
{
    int expecting_num = TRUE;
    int is_num = FALSE;

    /* Skipping on the labels name. */
    if(label[0] != '\0')
    {
        line += strlen(label) + 1;
    }

    /* Checking for ".data". */
    if(strncmp(line, ".data", strlen(".data")) != 0)
    {
        printf("In data instruction at line %d, there is no .data instruction. \n", count);
        return error;
    }

    line += strlen(".data"); /* Skipping on ".data". */

    while(*line != '\n' && *line != '\0')
    {
        if(expecting_num) /* If a num should follow we look for it. */
        {
            if(*line == '+' || *line == '-' || isdigit(*line))
            {
                is_num = TRUE;
                expecting_num = FALSE;
            }
            else /* No num found we print error. */
            {
                printf("In data instruction at line %d, there isn't a number defined. \n", count);
                return error;
            }
        }
        else
        {
            if(*line == ',') /* After a ',' a num should follow, and we will look for it. */
            {
                expecting_num = TRUE;
            }
            else if(!isdigit(*line)) /* If we found a different chatecter we will print an error. */
            {
                printf("In data instruction at line %d,"
                       " there is an undefined character. \n", count);
                return error;
            }
        }
        line++;
    }
    return !expecting_num && is_num; /* Returning if an error was found or not. */
}
