
#include "main.h"
#include "shell.h"
char* filename = null;
char* shellFile = null;
int commands_run = 1;
int testing  = 0;

/**
 * main - func
 * @argc : arg 
 * @argv : arg 
 * Return : type
*/
int main(int __attribute__((unused)) argc, const char __attribute__((unused)) *argv[])
{
    filename = strdup(argv[0]);
    testing = argc > 1 ? 1 : 0;
    if( isatty(STDIN_FILENO) && !testing)
        interactive();
    else 
    {
        shellFile = strdup(argv[testing]);
        noninteractive();
        free(shellFile);
    }    
    free(filename);
    return 0;
}


