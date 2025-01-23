#include "assembler_definitions.h"

void * my_malloc(long size)
{
    void *memory = malloc(size);
    if(memory == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return memory;
}

char *clean_line(char *line)
{
    char *new_line;
    int i, j, k;
    i = 0;
    j = 0;
    new_line = my_malloc(MAX_LINE_LENGTH);
    memset(new_line, '\0', MAX_LINE_LENGTH);
    for(; line[i] != '\0'; i++)
    {
        if(line[i] == '"')
        {
            new_line[j++] = line[i];
            i++;
            k = strlen(line) - 1;
            while(k > i && line[k] != '"') { k--; }
            if(i != k)
            {
                while(i <= k) { new_line[j++] = line[i++]; }
            }
        }
        if(line[i] == ';')
        {
            new_line[j++] = '\n';
            i = strlen(line);
        }
        if(line[i] != ' ' && line[i] != '\t') { new_line[j++] = line[i]; }
    }
    new_line[j] = '\0';
    return new_line;
}

char *edit_file_name(char *file_name, char *end)
{
    char *new_name = my_malloc(MAX_LINE_LENGTH);
    memset(new_name, '\0', MAX_LINE_LENGTH);
    strcpy(new_name, file_name);
    strcat(new_name, end);
    return new_name;
}
