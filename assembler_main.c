/**
 * This program is an assembler - converting assembly code into output files representing
 * each action in the assembly code.
 *
 * @author Koren Levy
 * @Id 216968727
 *
 * Working journal:
 *
 * 20/21 of december - reading the instructions for the first time, thinking about quitting.
 * 27/28 of december - the instructions are starting to make sense.
 * 3/4 of january - starting to think about how to handle memory. Breakthrough, lists.
 * 10/11 of january - writing the "assembler_definistions" file, and having faith everything works.
 * 17/18 of january - writing the helping functions needed for the pre proccess fase, and struggeling
 * with unnecessary characters in the macro names.
 * 24/25 of january - writing the pre proccess faze, surprisingly everything works out fine.
 * 31/1 of january/february - yeah the pre proccess has some problems, I fixed that.
 * 7/8 of february - writing the functions that adds to lists needed for the first pass. Took a while
 * because of again, some weird charecters.
 * 14/15 of february - trying to understand what words are and why do I need them. Adding them as a
 * type in "assembler definitions".
 * 21/22 of february - finally figured it out, writing the functions to creat a word for one
 * and two operands. Took way too long.
 * 28/1 of february/march - writing the function that makes words according to the instruction
 * using the other two functions. Started writing the first pass. A good weekend.
 * 7/8 of march - finished writing the first pass, not too much trouble.
 * 14/15 of march - I was sick, didn't do anything and didn't understand the second pass.
 * 21/22 of march - spended the whole weekend writing and changing it back because it didn't work.
 * Git-hub as backup helped tremendously. (English exam is tomorrow, trying to use big words).
 * 28/29 of march - now I get it, took me a few hours to write and a few more to fix the first pass again.
 * And again a few more to write help functions to print.
 * 30/31 of march - I spended five hours writing api's and a couple more to document.
 * Spended some time cleaning up.
 *
 * In conclusion, 5 stars would recommend.
 */


#include "assembler_definitions.h"
#include "pre_and_passages.h"
#include "Utility.h"

/**
 * Do you have to api the main function?
 * This function takes files from the command line and executes
 * an assembler on them. Outputing files as long as no error occur.
 *
 * To start the program type "make" and then "./assembler" and enter your files.
 *
 * @param argc The number of parameters in the command line.
 * @param argv The parameters from the command line in string format.
 * @return Success or fail value.
 */
int main(int argc, char *argv[])
{
    int i;
    char *file_name, *file_with_as; /* The name of the file read. */
    Assembly_Content *content = NULL;

    /* For each file given in the command line we will execute the assembler. */
    for(i = 1; i < argc; i++)
    {
        /* Memory allocation for the assembler lists. */
        content = my_malloc(sizeof(Assembly_Content));

        /* Initialization of the lists to be ready to use. */
        content->command_head = NULL;
        content->entry_head = NULL;
        content->extern_head = NULL;
        content->instruction_head = NULL;
        content->label_head = NULL;
        content->macro_head = NULL;

        /* Getting the file name, and adding the ".as" ending necessary. */
        file_name = argv[i];
        file_with_as = edit_file_name(file_name, ".as");

        printf("Executing file: %s\n", file_with_as);

        /* Starting the pre proc fase with the current file. */
        pre_assembler(&content, file_name, file_with_as);

        free_content(content); /* Freeing the memory allocated before the next file. */
    }

    return 0;
}
