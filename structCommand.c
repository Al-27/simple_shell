#include "main.h"
#include "shell.h"

Commands_st* newCommand(Commands_st *head, char* command)
{
    Commands_st *new = null, *cur = null;
    new = malloc(sizeof(Commands_st));
    new->next = null;
    new->commands = splitStr(command, 2, '&', '|');
    new->pLogicOps = getLogOps(command);
    new->offset_cmd = 0;
    
    if(!head)
    {
        return new;
    }
    
    cur = head;
    
    while(head->next)
    {
        head = head->next;
    }
    
    head->next = new;
    head = cur;
    
    return head;
}

int* getLogOps(char* str)
{
    int *logicals = null, i = 0, nLogics = 0;
    char lastc = ' ';
    
    if(!str)
    {
        return null;
    }
    
    nLogics = ( byte_occuren(str,'|')/2 + byte_occuren(str,'&')/2 );
    
    if(nLogics)
    {
        logicals = malloc( sizeof(int) * nLogics );
    }
    
    for(;*str && i < nLogics; str++)
    {
        if(*str != lastc )
        {
            if(*str == '|')
            {
                logicals[i] = OR;
                i++;
            }
            if(*str == '&')
            {
                logicals[i] = AND;
                i++;
            }
        }
        lastc = *str;
        
    }
    
    return logicals;
}

void free_st(Commands_st *st)
{
    if(st)
    {
        if(st->next)
        {
            free_st(st->next);
        }
        free_all(st->pLogicOps,st->commands);
        free(st);
        st = null;
    }
}

void cmdSeek(Commands_st *head, int start)
{
    if(head)
    {
        
        if(start)
        {
            if( head->offset_cmd ) head->commands -= head->offset_cmd;
            head->offset_cmd = 0;
        }
        else 
        {
            if(*(head->commands))
            {
                head->offset_cmd++;
                head->commands++;
            }
        }
        
    }
}

Commands_st* getLastElem(Commands_st *head)
{
    if(!head)
        return head;
        
    if(head->next)
        getLastElem(head->next);
    else   
        return head;
    
    return getLastElem(head->next);
}