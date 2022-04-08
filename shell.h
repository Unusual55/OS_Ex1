#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>

///////////////////Constants//////////////
#define SHELL_OUT 1
#define SERVER_OUT 2
#define TO_SERVER "TCP PORT"
#define TO_SHELL "LOCAL"
#define ECHO "ECHO"
#define PROMPT "I can do this all day"
#define MAX_LENGTH 256
#define EXIT "EXIT"
#define dir "DIR"
#define null NULL
////////////////////Functions////////////
char * get_prompt();
void reset_path();
char* get_path();
int input_check(char * input);
char* firstTrims(char* input);
int caseExit();
int caseEcho(char* token);
int caseDir();
int caseCd(char* token);
int caseCopy(char* token);
int caseDelete(char *token);
int caseTCP(char* token);
int caseLocal();
void resetArguments();
void setArguments();
#endif