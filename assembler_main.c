#include "assembler_definitions.h"
#include "pre_and_passages.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
    int i;
    char *file_name, *file_with_as;
    Assembler_Table *table = NULL;

    for(i = 1; i < argc; i++)
    {
        table = my_malloc(sizeof(Assembler_Table));

        table->command_head = NULL;
        table->entry_head = NULL;
        table->extern_head = NULL;
        table->instruction_head = NULL;
        table->label_head = NULL;
        table->macro_head = NULL;

        file_name = argv[i];
        file_with_as = edit_file_name(file_name, ".as");

        printf("Executing file: %s\n", file_with_as);

        pre_assembler(&table, file_name, file_with_as);

        free_table(table);
    }

    return 0;
}
