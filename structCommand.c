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
    new->offset_logic = 0;
    
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
    int *logicals = malloc( sizeof(int) * ( byte_occuren(str,'|')/2 + byte_occuren(str,'&')/2 ) + 1 ),
         i = 0;
    char lastc = ' ';
    
    for(;*str ; str++)
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
            head->commands -= head->offset_cmd;
            head->pLogicOps -= head->offset_logic;
            head->offset_cmd = 0;
            head->offset_logic = 0;
        }
        else 
        {
            if(*(head->commands))
            {
                head->offset_cmd++;
                head->commands += head->offset_cmd;
            }
            if(*head->pLogicOps)
            {
                head->offset_logic++;
                head->pLogicOps += head->offset_logic;
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