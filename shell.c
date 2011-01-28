#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/param.h>
#include "parse.h"   /*include declarations for parse-related structs*/

enum BUILTIN_COMMANDS {
	NO_SUCH_BUILTIN = 0, EXIT, JOBS
};

char* buildPrompt() {

	/* Decalre variables */
	char pwd[MAXPATHLEN];
/*	char * username;
	char hostname[MAXHOSTNAMELEN];*/
	char * prompt; 

	/* Grab the current working directory */
	getcwd( pwd, MAXPATHLEN );

/*	This was an attempt at grabbing the username and hostname to include in the prompt.  It works on the first run through
	but for some reason starts duplicating the prompt every time a command is run.

	Grab the current user 
	username = getenv("USER");
	if( username == NULL )
	{
		exit(1);
	}
	

	 Grab the current hostname 
	if( gethostname( hostname, MAXHOSTNAMELEN ) == -1 )
	{
		exit(1);
	}

	 Build up the prompt 
	prompt = strcat( username, "@" );
	prompt = strcat( prompt, hostname );
	prompt = strcat( prompt, ":" );
	prompt = strcat( prompt, pwd );
	prompt = strcat( prompt, "%" );
*/


	/*return prompt;*/
	return strcat(pwd, "%");
}

int isBuiltInCommand(char * cmd) {

	if (strncmp(cmd, "exit", strlen("exit")) == 0) {
		return EXIT;
	}
	return NO_SUCH_BUILTIN;
}

int main(int argc, char **argv) {

	/* Variable Declaration */
	int pid, status;
	char * cmdLine;
	ParseInfo *info; /*info stores all the information returned by parser.*/
	struct commandType *com; /*com stores command name and Arg list for one command.*/
	int returnCode;

#ifdef UNIX
	fprintf(stdout, "This is the UNIX version\n");
#endif

#ifdef WINDOWS
	fprintf(stdout, "This is the WINDOWS version\n");
#endif

	while (1) {
		/*insert your code to print prompt here*/

#ifdef UNIX
		cmdLine = readline(buildPrompt());
		if (cmdLine == NULL) {
			fprintf(stderr, "Unable to read command\n");
			continue;
		}
#endif

		/*insert your code about history and !x !-x here*/

		/*calls the parser*/
		info = parse(cmdLine);
		if (info == NULL) {
			free(cmdLine);
			continue;
		}
		/*prints the info struct*/
		print_info(info);

		/*com contains the info. of the command before the first "|"*/
		com = &info->CommArray[0];
		if ((com == NULL) || (com->command == NULL)) {
			free_info(info);
			free(cmdLine);
			continue;
		}
		/*com->command tells the command name of com*/
		if (isBuiltInCommand(com->command) == EXIT) {
			exit(1);
		}

		/*insert your code here.*/
	        pid = fork();
		if( pid == 0 )
		{
			returnCode = execvp(com->command, com->varList);
			if( returnCode == -1 )
			{
            			printf("Not a valid command\n");
        		}
		}
		else if (pid < 0)
		{
			/* If we get here it means that the fork() has failed */
			exit(1);
		}
		else
		{
			waitpid(pid, &status, 0);
		}

		free_info(info);
		free(cmdLine);
	}/* while(1) */
}
