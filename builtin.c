#include "main.h"
#include "shell.h"
#include "builtin.h"


int run_builtin(char* comm)
{
    int i = 0;
    char** arg = null, *pwd= null;
    builtin_t bi_t[] = 
    {
        {"exit", handler},
        {"env", handler}
        
    };
    
    
    for(i = 0; i < (int)(sizeof(bi_t)/sizeof(builtin_t)); i++)
    {
        if(strcmp(bi_t[i].command, comm) == 0){
            bi_t[i].func(i);
            return 0;    
        }
    }
    
    arg = get_args(comm);
    
    if( !strcmp("cd", arg[0]) )
    {
        arg++;
        if( *arg )
        {
            if(!strcmp(*arg,"-"))
            {
                chdir(getenv("HOME"));
                pwd = getcwd(NULL,0);
                setenv("PWD",pwd,1);
                free(pwd);
            }
            else
            {
                chdir(*arg);
                pwd = getcwd(NULL,0);
                setenv("PWD",pwd,1);
                free(pwd);
            }
            
        }
        else{ 
                chdir(getenv("HOME")); 
                pwd = getcwd(NULL,0);
                setenv("PWD",pwd,1);
                free(pwd);
        }
        arg--;
        free_all(null,arg);
        return 0;
    }
    else 
        free_all(null,arg);
    
    return 1;
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
