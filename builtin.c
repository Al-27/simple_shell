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
                if(chdir(getenv("OLDPWD")) != 0)
                {
                    fprintf(stderr,"%s",strerror(errno));
                } 
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
                 if(chdir(newpath) != 0)
                {
                    fprintf(stderr,"%s",strerror(errno));
                } 
                setenv("PWD",newpath,1);
                free(newpath);
                free(pwd);
            }
            
        }
        else{ 
                if(chdir(getenv("HOME")) != 0)
                {
                    fprintf(stderr,"%s",strerror(errno));
                } 
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
        exit(0);
        break;
    case 1:
        while(*env) fprintf(stdout,"%s\n",*env),env++;
        break;
    default:
        break;
    }
}
