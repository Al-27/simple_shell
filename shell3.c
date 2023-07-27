#include "main.h"
#include "shell.h"
#include "builtin.h"

extern char *filename;
extern char* shellFile;
extern int commands_run;
extern int testing;

char *trimAll(char *str)
{
    char *newStr = malloc(BUFF_SIZE), *beg = str, lastChar = 0;
    int i = 0, all_whitespace = 1, backward = 1;
    
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
    
    beg = newStr + i - 1;/* new we try to trim the end of the string */
    
    while(beg != newStr)
    {
        if(backward && *beg != ' ') break;
        
        if(*beg == ' ') i--;
        else{
            newStr[i] = *beg;
            i++;
            break;
        }
        backward=0;
        beg--;i--;
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
/* useless, and buggy (too much work to be done)
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
    cur -= i;
    free((cur));
    cur = null;
    
    return bin;
} */

void noninteractive()
{
    char* fd_buffer = null, **commands = null; 
    int eof = 1, cmdsLoop, newline ;
    Commands_st *command_st = null, *head;
    FILE* sh2 =null;/************************/
    
    cmdsLoop = 0;
    
    fd_buffer = malloc(BUFF_SIZE);
    
    if(!fd_buffer) return; 
    
    /*
        wait for, then read input
    */
    memset(fd_buffer,0,BUFF_SIZE);
    
    if(testing){ sh2=fopen(shellFile,"r"); eof = read(sh2->_fileno,fd_buffer,BUFF_SIZE);   fclose(sh2);}
    else eof = read(stdin->_fileno,fd_buffer,BUFF_SIZE);
    
    fd_buffer[eof] = '\0';
    
    if(eof > 1 ) 
        {
            
            fd_buffer = trimAll(fd_buffer);
            commands = splitStr(fd_buffer,2, '\n',';');
            if(commands){
                while( *commands )
                {
                    head = command_st = newCommand(command_st, *commands);
                    command_st = getLastElem(command_st);
                    newline = 1;     
                    
                    while( *command_st->commands ) 
                    {
                        handle_command(command_st);                    
                        cmdSeek(command_st,0);
                        commands_run++;
                        newline = 0;
                    }
                    
                    if(newline)
                        commands_run++;
                    
                    cmdSeek(command_st,1);
                    
                    
                    command_st = head;                    
                    commands++;
                    cmdsLoop++;
                }
                commands -= cmdsLoop;
            }
            {
                commands_run++;
            }
        
        }
        else
            commands_run++;
    
    free_st(command_st);
    free_all(fd_buffer,commands);
    command_st = head = null, commands = null;
    fd_buffer = null;
    
  
    /*if(eof == 0) write(STDOUT_FILENO, "\n",1);*/
}
