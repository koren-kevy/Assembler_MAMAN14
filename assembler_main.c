#include "assembler_definitions.h"
#include "pre_and_passages.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
    int i;
    char *file_name, *file_with_as;
    Assembler_Table *table = NULL;

    table = my_malloc(sizeof(Assembler_Table));

    table->command_head = NULL;
    table->entry_head = NULL;
    table->extern_head = NULL;
    table->instructions_head = NULL;
    table->label_head = NULL;
    table->macro_head = NULL;

    for(i = 1; i < argc; i++)
    {
        file_name = argv[i];
        file_with_as = edit_file_name(file_name, ".as");
        pre_assembler(&table, file_name, file_with_as);
    }

    free(table);

    return 0;
}
