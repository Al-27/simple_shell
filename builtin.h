
#ifndef _builtin_H
#define _builtin_H

typedef struct builtin
{
    char* command;
    void (*func)(int);
}builtin_t;

void (*run_builtin(char*));
void handler(int);
void print_env();

 
#endif
