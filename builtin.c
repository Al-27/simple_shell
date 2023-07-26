#include "main.h"
#include "builtin.h"


void (*run_builtin(char* comm))
{
    int i = 0;
    builtin_t bi_t[] = 
    {
        {"exit", handler},
        {"env", handler},
        
    };
    
    for(i = 0; i < (int)(sizeof(bi_t)/sizeof(builtin_t)); i++)
    {
        if(strcmp(bi_t[i].command, comm) == 0)
            bi_t[i].func(i);
    }
    
    return null; 
    
}


void handler(int i)
{
    char **env = __environ;
    
    switch (i)
    {
    case 0:
        exit(EXIT_SUCCESS);
        break;
    case 1:
        while(*env) fprintf(stdout,"%s\n",*env),env++;
        break;
    default:
        break;
    }
}
