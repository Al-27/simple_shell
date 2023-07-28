#include "main.h"
#include "shell.h"
#include "builtin.h"
#include <errno.h>

int run_builtin(char* comm)
{
    int i = 0;
    char** arg = null, *pwd= null, *newpath=null;
    builtin_t bi_t[] = 
    {
        {"exit", exitb},
        {"env", print_env}
        
    };
    
    arg = get_args(comm);
    
    for(i = 0; i < (int)(sizeof(bi_t)/sizeof(builtin_t)); i++)
    {
        if(strcmp(bi_t[i].command , *arg) == 0){
            bi_t[i].func(arg);
            free_all(null,arg);
            return 0;    
        }
    }
    
    
    if( !strcmp("cd", arg[0]) )
    {
        arg++;
        if( *arg )
        {
            if(!strcmp(*arg,"-"))
            {
                chdir(getenv("OLDPWD")) ;
                pwd = getcwd(NULL,0);
                setenv("PWD",pwd,1);
                free(pwd);
            }
            else
            {
                pwd = getcwd(NULL,0);
                newpath = malloc(4096+1);
                memset(newpath,0,4096);
                strcat(newpath,pwd); 
                strcat(newpath,*arg); 
                chdir(newpath) != 0;
                setenv("PWD",newpath,1);
                free(newpath);
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
    
    free_all(null,arg);
    
    return 1;
}

void print_env(char** arg)
{
    arg = __environ;
    while(*arg) fprintf(stdout,"%s\n",*arg),arg++;
}

void exitb(char** arg)
{
    arg++;
    
    if(*arg)
    {
        exit( strtol(*arg,null,10) );
    }
    
    exit(0);
}

