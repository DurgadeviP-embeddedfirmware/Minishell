#include"shell.h"
char *external_commands[200];
int exit_flag;
char prompt[100] = "minishell$";
char *builtins[] = {"echo","cd", "pwd","exit",NULL};
int pid;
extern Slist *head;
char input_string[100];
void handler(int num)
{
	//printf("SIGINT is Called to hanlder\n");
	printf("minishell$\n");
}

void handler2(int num)
{
	//  printf("SIGTSTP is Called to parent hanlder\n");
	//signal(SIGTSTP,SIG_IGN);
	printf("%s",prompt);
}

int main()
{
	Slist *head = NULL;
	//char input_string[100];
	//prompt[100] = "minishell$";
	system("clear");
	//printf("%s : %p\n", __func__, external_commands);
	int ret = extract_external_commands(external_commands);
	if(ret == 1)
	{
		printf("File not Opened\n");
		return 1;
	}
	signal(SIGINT,handler);
	signal(SIGTSTP,SIG_IGN);
	//signal(SIGTSTP,handler2);
	//printf("Get pid is %d\n",getpid());
	// struct sigaction sa;
	//sa.sa_handler = SIG_IGN;
	// sa.sa_handler = sa_handler;
	// sa.sa_flags = SA_NOCLDWAIT | SA_NOCLDSTOP;
	// sigemptyset(&sa.sa_mask);
	// sigaction(SIGCHLD, &sa, NULL);

	scan_input(prompt,input_string);
	// for(int i=0;external_commands[i]!=NULL;i++)
	// {
	// 	printf("%s ",external_commands[i]);
	// 	free(external_commands[i]);
	// }
	
}

