#include "main.h"
#include "shell.h"
#include "builtin.h"

extern char *filename;
extern int commands_run;

char *trimAll(char *str)
{
    char *newStr = malloc(BUFF_SIZE), *beg = str, lastChar = 0;
    int i = 0, all_whitespace = 1;
    
    if( !str )
        return null;
    
    while (*beg)
    {
        if(*beg == ' ' && (lastChar == ' ' || lastChar == 0 ) ) /*do nothing*/;
        else
        {
            newStr[i] = *beg;
            i++;
            all_whitespace = 0;
        }
        lastChar = *beg;
        beg++;
    }
    
    beg = newStr + i;/* new we try to trim the end of the string */
    beg--, i--;
    
    while(beg != newStr)
    {
        if(*beg == ' ');
        else{
            newStr[i] = *beg;
            i++;
            break;
        }
        *beg-=1,i-=1;
    }
    /*while(*(beg+i)) i++;*/
    newStr[i] = '\0';
    
    free(str);
    
    str = beg = null;
    
    if(all_whitespace){
        free(newStr);
        newStr = null;
    }    
    
    return newStr;
}

char *getExec(char* command)
{
    char *bin = malloc(32), *cur = null;
    int i = 0;
    
    if( !command )
        return null;
    
    cur = trimAll(strdup(command));
    while(*cur && *cur != ' ')
    {
        bin[i] = *cur;
        i++, cur++;
    }
    bin[i] = '\0';
    
    free((cur-i));
    cur = null;
    
    return bin;
} 

void noninteractive()
{
    char* fd_buffer = null, **commands = null; 
    int eof = 1, cmdsLoop ;
    Commands_st *command_st = null, *head;
    
    cmdsLoop = 0;
    
    fd_buffer = malloc(BUFF_SIZE);
    
    if(!fd_buffer) return; 
    
    /*
        wait for, then read input
    */
    memset(fd_buffer,0,BUFF_SIZE);
    eof = read(STDIN_FILENO,fd_buffer,BUFF_SIZE);
    fd_buffer[eof] = '\0';
    
    if(eof > 0 ) 
    {
        
        fd_buffer = trimAll(fd_buffer);
        commands = splitStr(fd_buffer,2, '\n',';');
        if( !**commands )
            commands_run++;
        
        while( *commands )
        {
            head = command_st = newCommand(command_st, *commands); 
            
            command_st = getLastElem(command_st);
            
            while( *command_st->commands && **command_st->commands) 
            {
                run_builtin( *command_st->commands );
                run_command( *command_st->commands );
                cmdSeek(command_st,0);
                commands_run++;
            }
                
            cmdSeek(command_st,1);
            
            
            command_st = head;                    
            commands++;
            cmdsLoop++;
        }
        commands -= cmdsLoop;
    
    }
    else
        commands_run++;
    
    free_st(command_st);
    free_all(fd_buffer,null);
    free_all(null,commands);
    command_st = null;
    head = null;
    commands = null;
    fd_buffer = null;
    
  
    /*if(eof == 0) write(STDOUT_FILENO, "\n",1);*/
}