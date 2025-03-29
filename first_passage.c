#include "assembler_definitions.h"
#include "error_handler.h"
#include "pre_and_passages.h"
#include "Utility.h"


void add_label(Label_List **list, char *label_name, int address, char *type)
{
    Label_List *new_label = my_malloc(sizeof(Label_List));

    label_name[strcspn(label_name, "\n\r\t ")] = 0;
    memset(new_label->label, '\0', MAX_LABEL_SIZE);
    memset(new_label->type, '\0', MAX_LABEL_SIZE);

    strcpy(new_label->label, label_name);
    strcpy(new_label->type, type);

    new_label->address = address;
    new_label->next = *list;
    *list = new_label;
}


void add_entry(Entry_List **list, char *entry)
{
    Entry_List *new_entry = my_malloc(sizeof(Entry_List));

    memset(new_entry->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_entry->label, entry);

    new_entry->next = *list;
    *list = new_entry;
    new_entry->add_list = NULL;
}


void add_extern(Extern_List **list, char *exter)
{
    Extern_List *new_extern = my_malloc(sizeof(Extern_List));

    memset(new_extern->label, '\0', MAX_LABEL_SIZE);
    strcpy(new_extern->label, exter);

    new_extern->next = *list;
    *list = new_extern;
    new_extern->add_list = NULL;
}


void add_instruction(Instruction_List **list, int address, Word word)
{
    Instruction_List *new_instruction = my_malloc(sizeof(Instruction_List));

    new_instruction->word = word;
    new_instruction->add = address;
    new_instruction->next = *list;

    *list = new_instruction;
}


void add_command(Command_List **list, int address, Word word, char label[MAX_LABEL_SIZE],
    int type)
{
    Command_List *new_command = my_malloc(sizeof(Command_List));

    memset(new_command->label, '\0', MAX_LABEL_SIZE);
    if(label != NULL)
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
        printf("No colon found in label definition at line %d. \n", line_count);
        return;
    }

    label_length = (int)(colon_ptr - line);

    strncpy(label, line, label_length);
    label[label_length] = '\0';
}


void get_string(Instruction_List **list, char *line, int line_count, int *address)
{
    int i;
    Word word;
    char *quotes = strstr(line, "\"");
    if(quotes == NULL)
    {
        printf("String definiation at line %d, has no quotes. \n", line_count);
        return;
    }

    line = quotes + 1;

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


void get_data(Instruction_List **list, char *line, int skip, int *address)
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
    if(line[0] == ':')
        line++;

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

            strcpy(command->name, instruction_names[i]);
            command->code = i;
            command->funct = instruction_functs[i];

            return INSTRUCTION;
        }
    }
    return NONE;
}


void make_word_for_one_operand_command(Command *command, Command_List **list, int *ic,
    char *dest, int dest_type)
{
    Word word;
    word.word = 0;

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
        add_command(list, *ic, word, dest, dest_type);
        (*ic)++;
    }
    else /* If its command or label. */
    {
        word.word = SECOND_PASS;
        add_command(list, *ic, word, dest, dest_type);
        (*ic)++;
    }
}


void make_word_for_two_operand_command(Command *command, Command_List **list, int *ic,
    char *source, int source_type, char *dest, int dest_type)
{
    Word word, other_word;
    word.word = 0;
    other_word.word = 0;

    word.word |= command->code << BITS_TO_MOVE_FOR_OPCODE;

    word.word |= source_type << BITS_TO_MOVE_FOR_SOURCE_TYPE;
    word.word |= dest_type << BITS_TO_MOVE_FOR_DEST_TYPE;

    word.word |= command->code << BITS_TO_MOVE_FOR_FUNCT;

    if(source_type == REGISTER)
    {
        word.word |= get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
    }

    if(dest_type == REGISTER)
    {
        word.word |= get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
    }

    /*word.word |= command->funct << BITS_TO_MOVE_FOR_FUNCT;*/

    add_command(list, *ic, word, source, source_type);
    (*ic)++;

    if(source_type != REGISTER)
    {
        if(source_type == LABEL)
        {
            other_word.word = SECOND_PASS;
        }
        else
        {
            other_word.word = get_operand_value(source_type, source) << BITS_TO_MOVE_FOR_SOURCE_REGISTER;
        }
        add_command(list, *ic, other_word, source, source_type);
        (*ic)++;
    }
    if(dest_type != REGISTER)
    {
        other_word.word = 0;

        if(dest_type == LABEL)
        {
            other_word.word = SECOND_PASS;
        }
        else
        {
            other_word.word = get_operand_value(dest_type, dest) << BITS_TO_MOVE_FOR_DEST_REGISTER;
        }
        add_command(list, *ic, other_word, dest, dest_type);
        (*ic)++;
    }
}

int make_command(Command_List **list, char *line, Command *command, int *ic, int count)
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

void first_passage(Assembler_Table **table_head, char *og_name, char *file_name)
{
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_SIZE];
    char *cleaned_line, *co_ptr;
    int line_count = 1;
    int ic = 100, dc = 1;
    int line_type, length, is_label;
    int flag = TRUE, is_error = no_error, total_error = no_error;
    Command *command = my_malloc(sizeof(Command));

    FILE *fptr = fopen(file_name, "r");
    if(fptr == NULL)
    {
        printf("Error opening file %s. \n", file_name); /* error */
        return;
    }


    command->code = 0;
    command->funct = 0;

    memset(command->name, '\0', MAX_LINE_LENGTH);
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
            is_label = check_for_same_label((*table_head)->label_head, label, line_count);

            is_error = check_space_and_colon(line, label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_legal_name(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_name_for_instruction(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
            is_error = check_name_for_register(label, line_count, FIRST_PASS);
            flag = flag && is_error == no_error;
        }

        line_type = type_line_first_pass(cleaned_line + strlen(label), command, &length);

        switch(line_type)
        {
            case DATA:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc - 1, ".data");
                }
                is_error = check_data_line(cleaned_line, line_count, label);
                flag = flag && is_error == no_error;
                if(flag)
                {
                    get_data(&(*table_head)->instruction_head, cleaned_line, length, &dc);
                }
                break;

            case STRING:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic + dc - 1, ".string");
                }
                if(flag)
                {
                    get_string(&(*table_head)->instruction_head, cleaned_line, line_count, &dc);
                }
                break;

            case ENTRY:
                if(is_label)
                {
                    sscanf(line + strlen(".entry") + 1, "%s", label);
                    add_label(&(*table_head)->label_head, label, ic + dc, ".entry");
                }
                if(flag)
                {
                    sscanf(line + strlen(".entry") + 1, "%s", label);
                    add_entry(&(*table_head)->entry_head, label);
                }
                break;

            case EXTERN:
                if(is_label)
                {
                    printf("In line %d, label is definied in extern definition.", line_count);
                }
                if(flag)
                {
                    sscanf(line + strlen(".extern") + 1, "%s", label);
                    add_extern(&(*table_head)->extern_head, label);
                }
                break;

            case INSTRUCTION:
                if(is_label)
                {
                    add_label(&(*table_head)->label_head, label, ic, "code");
                }
                if(flag)
                {
                    make_command(&(*table_head)->command_head, cleaned_line + length, command,
                        &ic, line_count);
                }
                break;

            case NONE:
                break;
        }
        total_error = total_error && flag == no_error;
        line_count++;
        flag = TRUE;
        memset(command->name, '\0', MAX_LINE_LENGTH);
        memset(line, '\0', MAX_LINE_LENGTH);
        memset(label, '\0', MAX_LABEL_SIZE);
    }
    free(command);
    fclose(fptr);
    fptr = NULL;

    reverse_list_label(&(*table_head)->label_head);
    reverse_list_entry(&(*table_head)->entry_head);
    reverse_list_extern(&(*table_head)->extern_head);
    reverse_list_command(&(*table_head)->command_head);
    reverse_list_instruction(&(*table_head)->instruction_head);


    second_pass(table_head, og_name, ic, dc, total_error);
}
