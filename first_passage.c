#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"
#include "Utility.h"


/**
 * This function adds a label to the head of the label list.
 */
void add_label(Label_List **list, char *label_name, int address, char *type)
{
    Label_List *new_label = my_malloc(sizeof(Label_List));

    /* Remove whitespace from label name. */
    label_name[strcspn(label_name, "\n\r\t ")] = 0;

    /* Initialize label name and type. */
    memset(new_label->label, '\0', MAX_LABEL_SIZE);
    memset(new_label->type, '\0', MAX_LABEL_SIZE);

    /* Copy parameters into the new node. */
    strcpy(new_label->label, label_name);
    strcpy(new_label->type, type);

    /* Add to the head of the list. */
    new_label->address = address;
    new_label->next = *list;
    *list = new_label;
}


/**
 * This function adds an entry to the head of the entry list.
 */
void add_entry(Entry_List **list, char *entry)
{
    Entry_List *new_entry = my_malloc(sizeof(Entry_List));

    /* Initialize entry name. */
    memset(new_entry->label, '\0', MAX_LABEL_SIZE);

    /* Copy parameters into the new node. */
    strcpy(new_entry->label, entry);

    /* Add to the head of the list. */
    new_entry->next = *list;
    *list = new_entry;
    new_entry->add_list = NULL;
}


/**
 * This function adds an extern to the head of the extern list.
 */
void add_extern(Extern_List **list, char *exter)
{
    Extern_List *new_extern = my_malloc(sizeof(Extern_List));

    /* Initialize extern name. */
    memset(new_extern->label, '\0', MAX_LABEL_SIZE);

    /* Copy parameters into the new node. */
    strcpy(new_extern->label, exter);

    /* Add to the head of the list. */
    new_extern->next = *list;
    *list = new_extern;
    new_extern->add_list = NULL;
}


/**
 * This function adds an instruction to the head of the instructions list.
 */
void add_instruction(Instruction_List **list, int address, Word word)
{
    Instruction_List *new_instruction = my_malloc(sizeof(Instruction_List));

    /* Copy parameters into the new node. */
    new_instruction->word = word;
    new_instruction->add = address;

    /* Add to the head of the list. */
    new_instruction->next = *list;
    *list = new_instruction;
}


/**
 * This function adds a command to the head of the commands list.
 */
void add_command(Command_List **list, int address, Word word, char label[MAX_LABEL_SIZE],
    int type)
{
    Command_List *new_command = my_malloc(sizeof(Command_List));

    /* Initialize label name. */
    memset(new_command->label, '\0', MAX_LABEL_SIZE);
    if(label != NULL)
        strcpy(new_command->label, label);

    /* Copy parameters into the new node. */
    new_command->add = address;
    new_command->binary_code = word;
    new_command->type = type;

    /* Add to the head of the list. */
    new_command->next = *list;
    *list = new_command;
}


/**
 * This functions extracts the name of a label from the line it's declared in.
 */
void get_label(char *line, char *label, char co ,int line_count)
{
    char *colon_ptr;
    int label_length;

    /* Skip to the colon in the line. */
    colon_ptr = strchr(line, co);
    if(colon_ptr == NULL)
    {
        printf("No colon found in label definition at line %d. \n", line_count);
        return;
    }

    /* Calculate the distance needed and coping the label into the parameter. */
    label_length = (int)(colon_ptr - line);

    strncpy(label, line, label_length);
    label[label_length] = '\0';
}


/**
 * This function extracts a string, adds the instruction, and updates the value of dc.
 */
void get_string(Instruction_List **list, char *line, int line_count, int *address)
{
    int i;
    Word word;

    /* Jumping to the quotes found. */
    char *quotes = strstr(line, "\"");
    if(quotes == NULL)
    {
        printf("String definiation at line %d, has no quotes. \n", line_count);
        return;
    }

    line = quotes + 1; /* Skipping the first quote. */

    /* Getting each character in the string and increasing dc, And adding the instructions. */
    for(i = 1; line[i] != '"' && line[i] != '\0'; i++)
    {
        word.word = (unsigned)line[i];
        add_instruction(list, *address, word);
        (*address)++;
    }
    word.word = 0;
    add_instruction(list, *address, word);
    (*address)++;
}


/**
 * This function extracts a data type, adds the instruction, and updates the value of dc.
 */
void get_data(Instruction_List **list, char *line, int skip, int *address)
{
    char *num;
    int int_num;
    Word word;
    line += skip; /* Skipping on the labels name. */

    num = strtok(line, ",\n"); /* Using tokens to see ','. */

    /* As long as there are numbers, we add instructions and update dc. */
    while(num != NULL)
    {
        int_num = atoi(num); /* Making the number from a string to an int. */
        if(int_num == 0 && num[0] == '0')
            return;
        word.word = int_num;
        add_instruction(list, *address, word);
        (*address)++;

        num = strtok(NULL, ",\n");
    }
}


/**
 * This function gets an operand and analyzes it.
 * It checks if it's a label, register, a number or none of them.
 */
int get_operand_type(char *operand)
{
    int i = 0;

    /* Checking for operand type to be a register. */
    for(; i < TOTAL_REGISTERS; i++)
    {
        if(strcmp(operand, register_names[i]) == 0)
        {
            return REGISTER;
        }
    }

    /* Checking for operand type to be a number. */
    if(operand[0] == '#')
    {
        return IMMEDIATE;
    }

    /* Looking for operand type to be a label. */
    if(isalpha(operand[0]))
    {
        return LABEL;
    }

    /* If there is no operand type. */
    return NO_TYPE;
}


/**
 * This function gets an operand and its type, and analyzes it.
 * It returns a value accurdenly.
 */
int get_operand_value(int type, char *operand)
{
    int value = 0;

    switch(type)
    {
        case REGISTER:
            /* If it's a right register we return the value. */
            if(operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
            {
                value = operand[1] - '0';
            }
            break;

        case IMMEDIATE:
            /* If it's a number we return the number. */
            value = atoi(operand + 1);
            break;

        case LABEL:
            value = 0;
            break;
    }
    return value;
}


/**
 * This function gets an assembly code and determines what happends in the line,
 * in order to know how to handle it.
 */
int type_line_first_pass(char *line, Command *command ,int *length)
{
    int i = 0;
    if(line[0] == ':') /* Skipping on ':'. */
        line++;


    /* We return the type of the instruction found in the begining of the line. */

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

    /* If it's an instruction, we update the parameters of "command" accordingly. */
    for(; i < TOTAL_INSTRUCTIONS; i++)
    {
        if(strncmp(line, instruction_names[i], strlen(instruction_names[i])) == 0)
        {
            *length = (int)strlen(instruction_names[i]);

            strcpy(command->name, instruction_names[i]);
            command->code = i;
            command->funct = instruction_functs[i];

            return INSTRUCTION;
        }
    }
    return NONE;
}


/**
 * This function creates a word to represent an assembly instruction or command,
 * and adds the command into the command list. A word that should be handled in the
 * second pass, is marked with a second pass symbol.
 * This function creates a word only for a one operand command.
 */
void make_word_for_one_operand_command(Command *command, Command_List **list, int *ic,
    char *dest, int dest_type)
{
    Word word;
    word.word = 0;

    /* Shifting bits for opcode and dest type. */

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    if(dest_type == REGISTER)
    {
        /* We get the right value and shift the bits. */
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
        add_command(list, *ic, word, dest, dest_type);
        (*ic)++; /* Update the value of ic. */
    }
    else /* If it's a command or a label we mark it for the second pass. */
    {
        word.word = SECOND_PASS;
        add_command(list, *ic, word, dest, dest_type);
        (*ic)++;
    }
}


/**
 * This function creates a word to represent an assembly instruction or command,
 * and adds the command into the command list. A word that should be handled in the
 * second pass, is marked with a second pass symbol.
 * This function creates a word only for a two operand command.
 */
void make_word_for_two_operand_command(Command *command, Command_List **list, int *ic,
    char *source, int source_type, char *dest, int dest_type)
{
    Word word, other_word;
    word.word = 0;
    other_word.word = 0;

    /* Shifting bits for opcode, source type, dest type and funct. */

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= source_type << BITS_TO_MOVE_FOR_SOURCE_TYPE;
    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    word.word |= command->code << BITS_TO_MOVE_FOR_FUNCT;

    if(source_type == REGISTER)
    {
        /* We get the value and shift the bits. */
        word.word |= get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
    }

    if(dest_type == REGISTER)
    {
        /* Again, we get the value and shift the bits. */
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    /* Add the command created and increase ic. */
    add_command(list, *ic, word, source, source_type);
    (*ic)++;

    if(source_type != REGISTER)
    {
        if(source_type == LABEL)
        {
            /*  If the source type is a label we will mark it for the second pass. */
            other_word.word = SECOND_PASS;
        }
        else
        {
            /* Else we will update the word by the value and shift the bits. */
            other_word.word = get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
        }

        /* Add the command created and increase ic. */
        add_command(list, *ic, other_word, source, source_type);
        (*ic)++;
    }

    if(dest_type != REGISTER)
    {
        other_word.word = 0;

        if(dest_type == LABEL)
        {
            /*  If the dest type is a label we will mark it for the second pass. */
            other_word.word = SECOND_PASS;
        }
        else
        {
            /* Else we will update the word by the value and shift the bits. */
            other_word.word = get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
        }

        /* Add the command created and increase ic. */
        add_command(list, *ic, other_word, dest, dest_type);
        (*ic)++;
    }
}


/**
 * This function generates a word representing assembly code using two
 * functions to assist. The function detects the instruction and creates the word accordingly.
 * Words that are created in the second pass are not handled.
 */
void make_command(Command_List **list, char *line, Command *command, int *ic, int count)
{
    int source_type = NO_TYPE, dest_type = NO_TYPE;
    char source[MAX_LABEL_SIZE], dest[MAX_LABEL_SIZE];

    /* Initialize source and dest. */
    memset(source, '\0', MAX_LABEL_SIZE);
    memset(dest, '\0', MAX_LABEL_SIZE);

    /* Looking for the right instruction. */
    switch(command->code)
    {
        /* Two operand commands. */
        case mov:
        case cmp:
        case add:
        case sub:
        case lea:

            /* Getting the source and the dest. */

            get_label(line, source,',', count);
            get_label(line + strlen(source) + strlen(","), dest, '\n', count);

            /* Getting the types and creating the word. */

            source_type = get_operand_type(source);
            dest_type = get_operand_type(dest);

            make_word_for_two_operand_command(command, list, ic, source, source_type, dest, dest_type );
            break;

        /* One operand commands. */
        case clr:
        case not:
        case inc:
        case dec:
        case jmp:
        case bne:
        case jsr:
        case red:
        case prn:

            /* Getting the source and the dest, and creating the word. */

            get_label(line, dest, '\n', count);
            dest_type = get_operand_type(dest);

            make_word_for_one_operand_command(command, list, ic, source, dest_type);
            break;

        /* Zero operand command, creating the word. */
        case rts:
        case stop:
            make_word_for_one_operand_command(command, list, ic, NULL, NO_TYPE);
            break;
    }
}


/**
 * This function handles an assembly file, by adding labels,
 * instructions, entry/extern commands or data/string commands to the according list.
 * Meanwhile, the function also calls for error checking functions.
 */
void first_passage(Assembly_Content **content_head, char *og_name, char *file_name)
{
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_SIZE];
    char *cleaned_line, *co_ptr;
    int line_count = 1;
    int ic = 100, dc = 1;
    int line_type, length, is_label;
    int flag = TRUE, is_error = no_error, total_error = no_error;
    Command *command = my_malloc(sizeof(Command));

    /* Opening the file to analyze. */
    FILE *fptr = fopen(file_name, "r");
    if(fptr == NULL)
    {
        printf("Error opening file %s. \n", file_name); /* error */
        return;
    }

    /* Initialize the command, label and line. */

    command->code = 0;
    command->funct = 0;

    memset(command->name, '\0', MAX_LINE_LENGTH);
    memset(line, '\0', MAX_LINE_LENGTH);
    memset(label, '\0', MAX_LABEL_SIZE);

    /* Read input file line by line. */
    while(fgets(line, MAX_LINE_LENGTH, fptr) != NULL)
    {
        cleaned_line = clean_line(line); /* A line with no white spaces. */
        is_label = FALSE;

        co_ptr = strstr(cleaned_line, ":"); /* Looking for a label. */
        if(co_ptr)
        {
            /* If there is a label we get its name and check for errors. */
            get_label(line, label, ':', line_count);
            is_label = check_for_same_label((*content_head)->label_head, label, line_count);

            is_error = check_space_and_colon(line, label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_legal_name(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_name_for_instruction(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_name_for_register(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
        }

        /* Checking if the line is a data/string/entry/extern/instruction/none. */
        line_type = type_line_first_pass(cleaned_line + strlen(label), command, &length);

        switch(line_type)
        {
            case DATA:
                if(is_label) /* If there is a label we add him with ".data". */
                {
                    add_label(&(*content_head)->label_head, label, ic + dc - 1, ".data");
                }
                is_error = check_data_line(cleaned_line, line_count, label);
                flag = flag && is_error == no_error;
                if(flag) /* If there was no error we add instructions and increase dc. */
                {
                    get_data(&(*content_head)->instruction_head, cleaned_line, length, &dc);
                }
                break;

            case STRING:
                if(is_label) /* If there is a label we add him with ".string". */
                {
                    add_label(&(*content_head)->label_head, label, ic + dc - 1, ".string");
                }
                if(flag) /* If there was no error we add instructions and increase dc. */
                {
                    get_string(&(*content_head)->instruction_head, cleaned_line, line_count, &dc);
                }
                break;

            case ENTRY:
                if(is_label) /* If there is a label we get him and add him with ".entry". */
                {
                    sscanf(line + strlen(".entry") + 1, "%s", label);
                    add_label(&(*content_head)->label_head, label, ic + dc, ".entry");
                }
                if(flag) /* If there was no error we add him to the entry list. */
                {
                    sscanf(line + strlen(".entry") + 1, "%s", label);
                    add_entry(&(*content_head)->entry_head, label);
                }
                break;

            case EXTERN:
                if(is_label) /* If there was a label we print an error. */
                {
                    printf("In line %d, label is definied in extern definition. \n", line_count);
                }
                if(flag) /* If there was no error we add him to the extern list. */
                {
                    sscanf(line + strlen(".extern") + 1, "%s", label);
                    add_extern(&(*content_head)->extern_head, label);
                }
                break;

            case INSTRUCTION:
                if(is_label) /* If there is a label we add him with ".code". */
                {
                    add_label(&(*content_head)->label_head, label, ic, "code");
                }
                if(flag) /* If there was no error we generate a command. */
                {
                    make_command(&(*content_head)->command_head, cleaned_line + length, command,
                        &ic, line_count);
                }
                break;

            case NONE:
                break;
        }
        total_error = total_error && flag == no_error;
        line_count++;
        flag = TRUE;

        /* Initialize the command name, label and line. */
        memset(command->name, '\0', MAX_LINE_LENGTH);
        memset(line, '\0', MAX_LINE_LENGTH);
        memset(label, '\0', MAX_LABEL_SIZE);
    }
    free(command);
    fclose(fptr);
    fptr = NULL;

    /* Because we added to the head of the lists we have to reverse them. */
    reverse_list_label(&(*content_head)->label_head);
    reverse_list_entry(&(*content_head)->entry_head);
    reverse_list_extern(&(*content_head)->extern_head);
    reverse_list_command(&(*content_head)->command_head);
    reverse_list_instruction(&(*content_head)->instruction_head);

    /* We move to the second pass, if there was an error we will not output files. */
    second_pass(content_head, og_name, ic, dc, total_error);
}
