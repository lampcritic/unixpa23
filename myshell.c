#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

//global variables

static char* arguments[512];
pid_t pid;
int command_with_pipe[2];
static char line[1024];
static int calls = 0;

static int commandProcess(int in, int f, int l)
{
	int pipeHandles[2];
	pipe(pipeHandles);
	pid = fork();
	
	//if returns to newly created child process
	if(pid == 0)
	{
		//if it's the first pipe command
		if((in == 0) && (f == 1) && (l == 0))
		{
			dup2(pipeHandles[WRITE], STDOUT_FILENO);
		}
	
		//if it's the middle pipe command
		else if(in != 0 && f == 0 && l == 0)
		{
			//takes output from first pipe and duplictes it in for
			// the middle
			dup2(in, STDIN_FILENO);
			dup2(pipeHandles[WRITE], STDOUT_FILENO);
		}
	
		//else if just the last command remaining
		else
			dup2(in, STDIN_FILENO);	
	}

	if(in != 0)
		close(in);
	close(pipeHandles[WRITE]);

	//will get here when its the last command, so it can close it
	if(l == 1)
		close(pipeHandles[READ]);

	return pipeHandles[READ];
}

static char* skipWhiteSpace(char* space)
{
	while(isspace(*space))
		space++;
	return space;
}

static int run(char* command, int input, int first, int last)
{
	command = skipWhiteSpace(command);
	char*  next = strchr(command, ' ');
	int i = 0;
	while(next != NULL)
	{
		next[0] = '\0';
		arguments[i] = command;
		i++;
		command = skipWhiteSpace(next + 1);
		next = strchr(command, ' ');
	}	
	if(command[0] != '\0')
	{
		arguments[i] = command;
		next = strchr(command, '\n');
		next[0] = '\0';
		i++;
	}

	arguments[i] = NULL;

	if(arguments[0] != NULL)
	{
		if(strcmp(arguments[0], "exit") == 0)
			exit(0);
		calls++;
		return commandProcess(input, first, last);	
	}
	else
		return 0;
}

int main()
{
	while(1)
	{
		if(!fgets(line, 1024, stdin))
			return 0;
		
		int input = 0;
		int first = 1;

		char* command = line;
		char* nextCommand = strchr(command, '|');
		
		//while there is a next command 
		while(nextCommand != NULL)
		{
			*nextCommand = '\0';
			input = run(command, input, first, 0);
			command = nextCommand++;
			nextCommand = strchr(command, '|');
			first = 0;	
		}
		input = run(command, input, first, 1);
		for(int i = 0; i < calls; i++)
			wait(NULL);
		calls = 0;  	
	}
	return 0;
}

//comment for week 09 homework
//comment for week 09 github changes
