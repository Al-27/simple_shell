
#ifndef _builtin_H
#define _builtin_H

typedef struct builtin
{
    char* command;
    void (*func)(char**);
}builtin_t;

int run_builtin(char*);
void print_env(char**);
void exitb(char**);

 
#endif
