#include "assembler_definitions.h"
#include "pre_and_passages.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
    int i;
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
        pre_assembler(&table, argv[i]);
    }

    free(table);

    return 0;
}
