#include"shell.h"
extern int exit_flag;
extern char *builtins[];
extern char *external_commands[200];
extern int pid;
int status;
extern char prompt[100]; 
Slist *head = NULL;
extern char input_string[100];
int count;
int p_flag;
void handle(int number)      //signal handler
{
	//printf(ANSI_COLOR_RED"sigint Handler for child\n");
	signal(SIGINT,SIG_DFL);
}
                                            //signal handler
void handle2(int number)
{
	//printf(ANSI_COLOR_RED"sigtstp Handler for child\n");
count++;
	printf("[%d] stopped ",count);
	if(strncmp(input_string,"sleep",5) == 0)
	{
           insert_at_last(&head,input_string,pid);
	   print(head,1);
	}
	//print(head,1);
	
}
void handle3(int signum)
{
	while(p_flag && (waitpid(p_flag,&status,WNOHANG) > 0))
	{
		delete_last(&head);
	}

}
int extract_external_commands(char **external_commands)     //external commands fun
{
	
	*external_commands[200];
	int fd = open("external_command.txt",O_RDONLY);
	if(fd == -1)
	{
		printf("File not opened\n");
		return 1;
	}
	for (int k = 0; k < 200; k++) 
	{
		external_commands[k] = (char *)calloc(20, sizeof(char));
		if (external_commands[k] == NULL)
		{
			printf("Memory allocation failed\n");
			return 1;
		}
	}
	int i=0,index=0,j=0,ret;
	char buf;
	char s[20];
	do
	{
		ret = read(fd,&buf,1);
		if(ret > 0)
		{
			if(buf == '\n')
			{
				s[index] = '\0';
				index = 0;
				strcpy(external_commands[j++],s);
			}
			else
			{
				s[index++] = buf;

			}
		}



	}while(ret > 0 && j<200);
	
	close(fd);
	
}

int scan_input(char *prompt, char *input_string)   //scan input
{
	while(1)
	{
		int co_flag = 0;
		static int flag;
		if(flag == 1)
			prompt = "minishell$";
		printf(ANSI_COLOR_YELLOW "%s"ANSI_COLOR_RESET,prompt);
		//scanf(" %[^\n]",input_string);
		if (fgets(input_string, 100, stdin) == NULL) continue;
            input_string[strcspn(input_string, "\n")] = '\0';
		//scanf(" %s",input_string);
		if(input_string[0] == '\n')
		{
			continue;
		}
		if(strcmp(input_string,"fg") == 0)
		{ 
			co_flag = 1;
			int pid2 = print(head,1);
			p_flag = pid2;
			if(pid2 == 4)
			{
				printf(ANSI_COLOR_GREEN"-bash: fg: current: no such job\n");
				continue;
			}
			kill(pid2,SIGCONT);
			// waitpid(pid2,&status,WUNTRACED);
			// if(WIFEXITED(status) ||  WIFSIGNALED(status))
			//     delete_last(&head);
			int wstatus;
			do {
				waitpid(pid2, &wstatus, WUNTRACED | WCONTINUED);
				if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus)) {
					delete_last(&head);
					break;
				}
			} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

		}

		else if(strcmp(input_string,"bg") == 0)
		{
			struct sigaction sa;
			//sa.sa_handler = SIG_IGN;
			sa.sa_handler = handle3;
	//sa.sa_flags = SA_NOCLDWAIT | SA_NOCLDSTOP;
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD, &sa, NULL);
			co_flag = 1;
			int pid = print(head,1);
			p_flag = pid;
			if(pid == 4)
			{
				printf(ANSI_COLOR_GREEN"-bash: bg: current: no such job\n");
				continue;
			}
			kill(pid,SIGCONT);
			
			//if(ret == -1)
			//	delete_last(&head);


		}

		else if(strcmp(input_string,"jobs") == 0)
		{
			co_flag = 1;
			int PID = print(head,3);
			if(PID == 4)
			{
				continue;
			}
			// if(WIFEXITED(status))
			// 	delete_last(&head);
		}
			
		if(strncmp(input_string,"PS1",3) == 0)
		{
			flag = 2;
		}
		else
		{
			flag = 6;
		}    	
		if(flag == 2)
		{
			char str[20] = "PS1=";
			int j;
			for(j=0;j<strlen(str);j++)
			{
				if(str[j] != input_string[j])
				{
					flag = 1;
					printf(ANSI_COLOR_YELLOW"Invalid Command\n");
					continue;
				}
			}
			if(input_string[j] != ' ')
			{  
				if(flag == 2)
				{
				    flag = 0;
				}
			}
			if(flag == 5)
			{
				flag = 2;
			}
		}
					
		fflush(stdin);
		int i;
		if(flag == 0)
		{
			memset(prompt,'\0',sizeof(prompt));
		  char str1[20] = "PS1=";
		        for(i=0;i<strlen(str1);i++)
		        {
			    if(str1[i] != input_string[i])
			     {
				flag = 1;
			     } 
	        	}
		}
		if(flag == 0)
		{
			if(input_string[i] != ' ')
			{
				for(int j=0;input_string[i]!='\0';j++)
				{
					prompt[j] = input_string[i++];
				}
				flag = 2;
				

				continue;
			}
			else
			{
				flag = 0;
			}
		}
		else if(flag == 1)
		{
			printf(ANSI_COLOR_YELLOW"Pass the PS1 command Properly\n");
			
			continue;
		}
		else if(flag == 2 || flag == 6) 
		{
			char *cmd = get_command(input_string);
	

			int ret1 = check_command_type(cmd);
			
			if(ret1 == EXTERNAL)
			{
				pid = fork();
				if(pid > 0)
				{
					signal(SIGTSTP,handle2);
					waitpid(pid,&status,WUNTRACED);
				        pid = 0;	

				}
				else
				{
					signal(SIGINT,handle);
					
					signal(SIGTSTP,SIG_DFL);
					int ret5 = execute_external_commands(input_string);
					if(ret5 == 1)
					{
						printf(ANSI_COLOR_YELLOW"execute_external_command failed\n");
						return 1;
					}
					exit(0);
				}	


			}
			else if(ret1 == BUILTIN)
			{
				int ret4 = execute_internal_commands(input_string);
				if(ret4 == 1)
				{
					printf(ANSI_COLOR_YELLOW"execute_internal_command failed\n");
					return 1;
				}

				
				if(exit_flag)
				{
					if(WIFEXITED(status))
					{
						printf("Exit status is %d\n",WEXITSTATUS(status));
					}
					else
					{
						printf("Abnormal condition\n");
					}
				}
				
			}
			else if(co_flag != 1)
			{
				printf(ANSI_COLOR_YELLOW"Invalid commands passed\n");
				
				continue;
			}
		
		}
	}
}



	char *get_command(char *input_string)
	{
		char *arr;
		char str[50];
		arr = (char*)malloc(strlen(input_string) * sizeof(char));
		int i;
		for(i=0;input_string[i] != ' ' && input_string[i] != '\0';i++)
		{
			str[i]  = input_string[i];
		}
		str[i] = '\0';
		return strcpy(arr,str); 
		
	}

	int check_command_type(char *command)
	{
		int i =0;
		
		for(int i=0;builtins[i] != NULL;i++)
		{
			if(strcmp(builtins[i],command) == 0)
			{
				return BUILTIN;
			}
			
		}
	
		for(i=0;external_commands[i] != NULL;i++)
		{
			
			if(strcmp(external_commands[i],command) == 0)
			{
				
				return EXTERNAL;
			}

		}
		return NO_COMMAND;
	}	

int execute_external_commands(char *input_string)
{
	
	char *str[200];

	for (int k = 0; k < 200; k++) 
	{
		
		str[k] = (char *)malloc(20*sizeof(char));
		if (str[k] == NULL)
		{
			printf("Memory allocation failed\n");
			return 1;
		}
	}
	int i=0,ind=0,j=0,ret;
	char buf	;
	char s[20];
	int pipe_flag=0;

	for(int i=0;i<strlen(input_string);i++)    
	{ 
		if(input_string[i] == ' ' || input_string[i+1] == '\0') 
		{
			if(input_string[i+1]=='\0') { 
				s[ind++] = input_string[i];
			}

			s[ind] = '\0';
			ind = 0;
			strcpy(str[j++],s);
		}
		else
		{
			s[ind++] = input_string[i];
		}


		if(strcmp(s,"|") == 0)
		{
			pipe_flag++;
		}

	}
	str[j]= '\0';
	if(pipe_flag != 0)
	{
		int ret = n_pipe(str,j);
		if(ret == 1)
		{
			printf("failed pipe fun\n");
			return 1;
		}
	}
	else
	{
		execvp(str[0],&str[0]);
	}
}

int n_pipe(char *arg[],int count)
{
	int num_pipes=0;
	for(int i=0;i<count;i++)
	{
		if(strcmp(arg[i],"|") == 0) {
			num_pipes++;
		}
	}
	if(num_pipes == 0)
	{
		printf("NO PIPES IN COMMAND LINE ARGS\n");
		return 1;
	}

	int j=1;
	int cmd_pos[num_pipes+1];
	cmd_pos[0] = 0;
	int pipe_cnt=0;
	for(int i=0;i<count;i++)
	{
		if(strcmp(arg[i],"|") == 0)
		{
			arg[i] = NULL;
			cmd_pos[j] = i + 1;
			pipe_cnt++;
			j++;
		}
	}
	int flag2 = 2;
	for(int i=0;i<pipe_cnt+1;i++)
	{
		int fd[2];
		if(i != pipe_cnt)
		{
			pipe(fd);
			flag2 = 1;
		}
		else
		{
			flag2=0;
		}
		int ret = fork();
		if(ret == 0)
		{
			close(fd[0]);
			if(flag2 == 1)
			{
				dup2(fd[1],1);
				close(fd[1]);
			}

			int ret4 = execvp(arg[cmd_pos[i]],arg + cmd_pos[i]);
			if(ret4 == -1)
			{
				printf(ANSI_COLOR_YELLOW"Invalid command\n");
				return 1;
			}
		}
		else
		{

			if(flag2 == 1)
			{
				close(fd[1]);
				dup2(fd[0],0);
				close(fd[0]);

			}
			waitpid(ret,NULL,0);
		}
	}
}
int execute_internal_commands(char *input_string)
{
	char arr[50];
	if(strcmp(input_string,"exit") == 0)
	{
		exit(0);
	}
	else if(strcmp(input_string,"pwd")==0)
	{
		getcwd(arr,50);
		printf(ANSI_COLOR_BLUE "%s\n",arr);
	}
	else if(strcmp(input_string,"echo $?")==0)
	{
		exit_flag = 1;
	}
	else if(strcmp(input_string,"echo $$") == 0)
	{ 
		printf(ANSI_COLOR_BLUE "Minishell Id is %d\n",getpid());
	}
	else if(strcmp(input_string,"echo $SHELL") == 0)
	{

		char *ptr = getenv("SHELL");
		printf(ANSI_COLOR_BLUE "%s\n",ptr);
	}
	else
	{
	        const char *buffer = strchr(input_string,' ');
		if(buffer != NULL)
			buffer++;
		int ret5 = chdir(buffer);
		if(ret5 != 0)
		{
			printf(ANSI_COLOR_MAGENTA"chdir failed , pass the write path....\n");
		}
		char cwd[1024];
		getcwd(cwd,sizeof(cwd));
		printf(ANSI_COLOR_CYAN"cwd is ---> %s\n",cwd);
	}
}
int delete_last(Slist **head)
{

	if(*head == NULL)
	{
		return 0;
	}
	Slist *temp = *head;
	Slist *prev;
	if(temp->link == NULL)
	{
		free(*head);
		*head = NULL;
		return 0;
	}
	while(temp->link != NULL)
	{
		prev = temp;
		temp = temp->link;
	}

	free(temp);
	prev->link = NULL;
	return 0;

}
int insert_at_last(Slist **head,char *input_string,int PID)
{
	//printf("Came to insert_last fun def\n");
	Slist *node = malloc(sizeof(Slist));
	node->link = NULL;
	node->pid = PID;
	strcpy(node->command,input_string);
	if(*head == NULL)
	{
	//	strcpy(node->command,input_string);
		*head = node;
		return 1;
	}
	Slist *temp = *head;
	while(temp->link != NULL)
	{

		temp = temp->link;
	}
	temp->link = node;
	
	
	return 1;
}
int print(Slist *head,int p_flag)
{
	Slist *temp = head;
	Slist *prev = NULL;
	if(head == NULL)
	{
		//printf(ANSI_COLOR_CYAN"No data in list\n");
		//exit(0);
		return 4;
	}
	else {
		while(temp != NULL)
		{
			if(p_flag == 3)
				printf(ANSI_COLOR_GREEN "%s\n",temp->command);
				//temp = temp->link;
			prev = temp;
			temp = temp ->link;

		}
		if(p_flag == 1)
		{
		    printf(ANSI_COLOR_GREEN "%s\n",prev->command);
		    p_flag = 0;
		    return prev->pid;
		}
	}
}


	
	
















