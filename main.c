
#include "main.h"
#include "shell.h"
char* filename = null;
int commands_run = 1;
int testing  = 0;

int main(int __attribute__((unused)) argc, const char __attribute__((unused)) *argv[], char __attribute__((unused)) *envp[])
{
    filename = strdup(argv[0]);
    
    if( isatty(STDIN_FILENO) && !testing)
        interactive();
    else 
        noninteractive();
         
    free(filename);
    return 0;
}


