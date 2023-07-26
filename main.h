#ifndef _main_H
#define _main_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stddef.h> 
#include <stdarg.h> 
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 256
#define MAX_ARGS 20
#define null ((void *)0)
#define OR 0x01
#define AND 0x02

typedef struct ShellCommands Commands_st;

struct ShellCommands
{
    char** commands;
    int* pLogicOps;
    int failed; /* Command failed to execute (ie. not found)*/
    int offset_cmd;
    Commands_st *next;
} ;

int* getLogOps(char* str);
Commands_st* newCommand(Commands_st *head, char* command);
void cmdSeek(Commands_st *head, int start);
void free_st(Commands_st *st);
Commands_st* getLastElem(Commands_st *head);

#endif