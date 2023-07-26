#include "main.h"
#include "shell.h"
#include "builtin.h"
extern int commands_run;
extern char* filename;

/**
 * init_shell - shell loop, handle commands/args, then free all;
 * 
 * 
 */
void interactive()
{
    char* fd_buffer = null, **commands = null ; 
    int eof = 1, cmdsLoop, newline;
    pid_t papaID = getpid();
    Commands_st *command_st = null, *head;
    
    while(eof != 0 && papaID == getpid())
    {
        eof = 1;
        cmdsLoop = 0;
        
        fd_buffer = malloc(BUFF_SIZE);
        
        if(fd_buffer == null) break; 
        
        /*
            wait for, then read input
        */
        memset(fd_buffer,0,BUFF_SIZE);
        write(STDOUT_FILENO,"($) ",5);
        eof = read(STDIN_FILENO,fd_buffer,BUFF_SIZE);
        fd_buffer[eof] = '\0';
        
        if(eof > 1 ) 
        {
            
            fd_buffer = trimAll(fd_buffer);
            commands = splitStr(fd_buffer,2, '\n',';');
            
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
        else
            commands_run++;
        
        free_st(command_st);
        free_all(fd_buffer,commands);
        command_st = head = null;
        commands = null;
        fd_buffer = null;
    }
     
    /*if(eof == 0) write(STDOUT_FILENO, "\n",1);*/
}

void handle_command(Commands_st* cmd_st)
{
    int offset = cmd_st->offset_cmd;
    if( offset == 0)
    {
        cmd_st->failed = run_builtin( *cmd_st->commands ) ?
                                run_command( *cmd_st->commands ) : 0;
    }
    else
    {
         if( !cmd_st->failed && cmd_st->pLogicOps[offset-1]  == OR ) ;
         else {
            cmd_st->failed = run_builtin( *cmd_st->commands ) ?
                                    run_command( *cmd_st->commands ) : 0;
        }
    }
}

int run_command(char *cm_st)
{
    char *command = null, **arg = null;
    int stat_loc = 0;
    pid_t child = -1;
    
    arg = get_args(cm_st);
    command = strdup(arg[0]);
    command = does_exist(command);
    
    if(!command || !arg) return 0;
    
    child = fork();
    
    if( child == 0){ 
        
        if( execve(command,arg,__environ) < 0 )
        {
            fprintf(stdout,"%s: %u: %s: not found\n",filename,commands_run,arg[0]);
            exit(1);
        }
    }
    else {
        do
        {
            waitpid(child,&stat_loc,WUNTRACED);
        } while (!WIFEXITED(stat_loc) && !WIFSIGNALED(stat_loc));
        WEXITSTATUS(stat_loc);
        
        stat_loc = WEXITSTATUS(stat_loc);
    }
    
    free_all(command,arg);
    return stat_loc;
}

void trim(char **str,int get_exec)
{
    char *trimBeg = *str, *trimEnd = null;
    int diff = 0;
    
    while(*trimBeg != '\0' && *trimBeg == ' ' && *str[0] == ' ')
    {
        trimBeg++;
    }
    
    trimBeg = strdup(trimBeg);
    free(*str);
    *str = trimBeg;
    
    if(get_exec)
    {
        while( *trimBeg != ' ' && *trimBeg != '\0'  ) trimBeg++;
        
        if( *trimBeg != '\0' )
        {
            diff = trimBeg - (*str);
            *str += diff;
            **str = '\0';
            *str -= diff;
        }
    }
    else
    {
        trimEnd = *str;
        while(*trimEnd) trimEnd++;
        
        while (*trimEnd > ' ')
        {
            trimEnd--;
        }
        diff = trimEnd + 1 - *str;
        *str[diff] = '\0';
        
    }
}

char** get_args(char* cmnd)
{
    char* temp = null, *first_char= null,  **args = malloc(sizeof(char* ) * MAX_ARGS);
    int diff = 0,/* exec_file = 1,*/ i = 0;
    void* p = null;
    
    if(cmnd)
    {
        temp = strdup(cmnd);
        p = temp;
    }
    
    while(temp && *temp)
    {
        if(*temp == '\n' && *(temp+1 )== '\0')
            *temp = ' ';
                
        if(*temp == ' ')
        {
            if(first_char != null)
            {
                diff = temp - first_char; 
                
                *temp = '\0';
                temp -= diff;
                args[i] = strdup(temp);
                temp += diff;
                *temp = ' ';
                first_char = null;
                i++;
            }
            
        }
        else
        {
            if(first_char == null)
            {
                first_char = temp;
            }
            
        }
        
        temp++;    
    }
    
    
    /*while(*temp)
    {
        if(*temp == '\n' && *(temp+1 )== '\0')
            *temp = ' ';
                
        if(*temp == ' ')
        {
            if(first_char != null && !exec_file)
            {
                diff = temp - first_char; 
                
                *temp = '\0';
                temp -= diff;
                args[i] = strdup(temp);
                temp += diff;
                *temp = ' ';
                first_char = null;
                i++;
            }
            
            if(exec_file)
            {
                exec_file = 0;
            }
        }
        else
        {
            if(first_char == null && !exec_file)
            {
                first_char = temp;
            }
            
        }
        
        temp++;    
    }*/
    if(first_char)
    {                  
        diff = temp - first_char; 
                
        temp -= diff;
        args[i] = strdup(temp);
        i++;
        
        first_char = null;
    } 
    args[i] = NULL;
    
    /*
        free string temp which we stepdupped
    */
    free(p);
    if(args[0] == null)
         args = null;
    return args;
}
