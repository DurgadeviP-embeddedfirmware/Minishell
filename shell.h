#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
//int exit_flag = 0;
typedef struct Node
{
	int pid;
	char command[100];
	struct Node *link;
}Slist;
int scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
int execute_internal_commands(char *input_string);
//void signal_handler(int sig_num);
int extract_external_commands(char **external_commands);
int execute_external_commands(char *input_string);
int n_pipe(char *arg[],int count);
int insert_at_last(Slist **head,char *input_string,int PID);
int delete_last(Slist **head);
int print(Slist *head,int );

#endif
