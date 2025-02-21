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
    char entry[MAX_LABEL_SIZE];

    get_label(line, entry, '\n', line_count);
    if(entry[0] == '\0')
        return;

    memset(new_entry->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_entry->label, entry);

    new_entry->next = *list;
    *list = new_entry;
    new_entry->add_list = NULL;
}


void add_extern(Extern_List **list, char *line, int line_count)
{
    Extern_List *new_extern = my_malloc(sizeof(Extern_List));
    char exter[MAX_LABEL_SIZE];

    get_label(line, exter, '\n', line_count);
    if(exter[0] == '\0')
        return;

    memset(new_extern->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_extern->label, exter);

    new_extern->next = *list;
    *list = new_extern;
    new_extern->add_list = NULL;
}


void add_instruction(Machine_Code_Instructions **list, int address, Word word)
{
    Machine_Code_Instructions *new_instruction = my_malloc(sizeof(Machine_Code_Instructions));

    new_instruction->word = word;
    new_instruction->add = address;
    new_instruction->next = *list;

    *list = new_instruction;
}


void add_command(Machine_Code_Command **list, int address, Word word, char label[MAX_LABEL_SIZE],
    int type)
{
    Machine_Code_Command *new_command = my_malloc(sizeof(Machine_Code_Command));

    memset(new_command->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_command->label, label);

    new_command->add = address;
    new_command->binary_code = word;
    new_command->type = type;

    new_command->next = *list;
    *list = new_command;
}


void get_label(char *line, char *label, char co ,int line_count)
{
    char *colon_ptr;
    int label_length;

    colon_ptr = strchr(line, co);
    if(colon_ptr == NULL)
    {
        printf("No colon found in label definition at line %d \n", line_count);
        return;
    }

    label_length = (int)(colon_ptr - line);

    strncpy(label, line, label_length);
    label[label_length] = '\0';
}


int get_string(Machine_Code_Instructions **list, char *line, int skip, int *address)
{
    int i;
    Word word;
    line += skip;

    if(line[0] != '"')
        return error;

    for(i = 1; line[i] != '"' && line[i] != '\0'; i++)
    {
        word.word = (unsigned)line[i];
        add_instruction(list, *address, word);
        (*address)++;
    }
    word.word = 0;
    add_instruction(list, *address, word);
    (*address)++;

    return no_error;
}


void get_data(Machine_Code_Instructions **list, char *line, int skip, int *address)
{
    char *num;
    int int_num;
    Word word;
    line += skip;

    num = strtok(line, ",\n");

    while(num != NULL)
    {
        int_num = atoi(num);
        if(int_num == 0 && num[0] == '0')
            return;
        word.word = int_num;
        add_instruction(list, *address, word);
        (*address)++;

        num = strtok(NULL, ",\n");
    }
}


int get_operand_type(char *operand)
{
    int i = 0;

    for(; i < TOTAL_REGISTERS; i++)
    {
        if(strcmp(operand, register_names[i]) == 0)
        {
            return REGISTER;
        }
    }

    if(operand[0] == '#')
    {
        return IMMEDIATE;
    }

    if(isalpha(operand[0]))
    {
        return LABEL;
    }

    return NO_TYPE;
}


int get_operand_value(int type, char *operand)
{
    int value = 0;

    switch(type)
    {
        case REGISTER:
            if(operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
            {
                value = operand[1] - '0';
            }
            break;

        case IMMEDIATE:
            value = atoi(operand + 1);
            break;

        case LABEL:
            value = 0;
            break;
    }
    return value;
}


int type_line_first_pass(char *line, Command *command ,int *length)
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
            *length = (int)strlen(instruction_names[i]);
            command->name = instruction_names[i];
            command->code = i;
            command->funct = instruction_functs[i];
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


void make_word_for_one_operand_command(Command *command, Machine_Code_Command **list, int *ic,
    char *dest, int dest_type)
{
    Word word;
    word.word = 0;

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    add_command(list, *ic, word, dest, dest_type);
    (*ic)++;

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    add_command(list, *ic, word, dest, dest_type);
    (*ic)++;
}


void make_word_for_two_operand_command(Command *command, Machine_Code_Command **list, int *ic,
    char *source, int source_type, char *dest, int dest_type)
{
    Word word;
    word.word = 0;

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= source_type << BITS_TO_MOVE_FOR_SOURCE_TYPE;
    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    if(source_type == REGISTER)
    {
        word.word |= get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
    }

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    word.word |= command->funct << BITS_TO_MOVE_FOR_FUNCT;

    add_command(list, *ic, word, source, source_type);
    (*ic)++;

    if(source_type == REGISTER)
    {
        word.word |= get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
    }

    add_command(list, *ic, word, source, source_type);
    (*ic)++;

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    add_command(list, *ic, word, dest, dest_type);
    (*ic)++;
}

int make_command(Machine_Code_Command **list, char *line, Command *command, int *ic, int count)
{
    int source_type = NO_TYPE, dest_type = NO_TYPE;
    char source[MAX_LABEL_SIZE], dest[MAX_LABEL_SIZE];

    memset(source, '\0', MAX_LABEL_SIZE);
    memset(dest, '\0', MAX_LABEL_SIZE);


    switch(command->code)
    {
        case mov:
        case cmp:
        case add:
        case sub:
        case lea:
            get_label(line, source,',', count);
            get_label(line + strlen(source) + strlen(","), dest, '\n', count);

            source_type = get_operand_type(source);
            dest_type = get_operand_type(dest);

            make_word_for_two_operand_command(command, list, ic, source, source_type, dest, dest_type );
            break;

        case clr:
        case not:
        case inc:
        case dec:
        case jmp:
        case bne:
        case jsr:
        case red:
        case prn:
            get_label(line, dest, '\n', count);
            dest_type = get_operand_type(dest);

            make_word_for_one_operand_command(command, list, ic, source, dest_type);
            break;

        case rts:
        case stop:
            make_word_for_one_operand_command(command, list, ic, NULL, NO_TYPE);
            break;

        default:
            return FALSE;
    }
    return TRUE;
}

void first_passage(Assembler_Table **table_head, char *file_name)
{
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_SIZE];
    char *cleaned_line, *co_ptr;
    int line_count = 1;
    int ic = 100, dc = 0;
    int line_type, length, is_label;
    int flag = TRUE, is_error = no_error;
    Command *command = my_malloc(sizeof(Command));

    FILE *fptr = fopen(file_name, "r");
    if(fptr == NULL)
    {
        printf("Error opening file %s \n", file_name); /* error */
        return;
    }

    command->name = NULL;
    command->code = 0;
    command->funct = 0;

    memset(line, '\0', MAX_LINE_LENGTH);
    memset(label, '\0', MAX_LABEL_SIZE);

    while(fgets(line, MAX_LINE_LENGTH, fptr) != NULL)
    {
        cleaned_line = clean_line(line);
        is_label = FALSE;

        co_ptr = strstr(cleaned_line, ":");
        if(co_ptr)
        {
            get_label(line, label, ':', line_count);
            is_label = TRUE;
        }
        line_type = type_line_first_pass(line + strlen(label), command, &length);

        switch(line_type)
        {
            case DATA:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc - 1);
                }
                /* Check for errors in data. */
                if(flag)
                {
                    get_data(&(*table_head)->instructions_head, line, strlen(label), &dc);
                }
                break;

            case STRING:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc - 1);
                }
                if(flag)
                {
                    get_string(&(*table_head)->instructions_head, line, strlen(label), &dc);
                }
                break;

            case ENTRY:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc);
                }
                if(flag)
                {
                    add_entry(&(*table_head)->entry_head, line + strlen(label), line_count);
                }
                break;

            case EXTERN:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc);
                }
                if(flag)
                {
                    add_extern(&(*table_head)->extern_head, line + strlen(label), line_count);
                }
                break;

            case INSTRUCTION:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic);
                }
                if(flag)
                {
                    make_command(&(*table_head)->command_head, line + strlen(label), command,
                        &ic, line_count);
                }
                break;

            case NONE:
                break;
        }

        line_count++;
        flag = TRUE;
        memset(line, '\0', MAX_LINE_LENGTH);
        memset(label, '\0', MAX_LABEL_SIZE);
    }
    free(command);
    fclose(fptr);
    fptr = NULL;
}
