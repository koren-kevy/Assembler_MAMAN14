#include "assembler_definitions.h"
#include "pre_and_passages.h"
#include "error_handler.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
    Assembler_Table *table = NULL;

    table = my_malloc(sizeof(Assembler_Table));

    table->command_head = NULL;
    table->entry_head = NULL;
    table->extern_head = NULL;
    table->label_head = NULL;
    table->macro_head = NULL;

    pre_assembler(&table, argv[1]);

    return 0;
}
