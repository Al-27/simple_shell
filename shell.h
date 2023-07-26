#ifndef _shell_H
#define _shell_H 
 

void interactive();
void handle_command(Commands_st* cmd_st);
int run_command(char*);
void trim(char** str,int get_exec);
char** get_args(char* comnd);

/*
    shell2.c
*/
void free_all(void*, char**);
int byte_occuren(char* str, char c);
char* getEnvVar();
char** splitStr(char* path,int nDelim, ...);
char* does_exist(char* filename);

/*
    shell3.c
    
char *getExec(char* command);
*/
void noninteractive();
char *trimAll(char *str);
#endif