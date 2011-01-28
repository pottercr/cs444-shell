#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/param.h>
#include "parse.h"   /*include declarations for parse-related structs*/
#include <sys/wait.h>

enum BUILTIN_COMMANDS {
	NO_SUCH_BUILTIN = 0, EXIT, JOBS, CD, KILL
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
	else if( strncmp(cmd, "jobs", strlen("jobs")) == 0) {
		return JOBS;
	}
	else if( strncmp(cmd, "cd", strlen("cd")) == 0) {
		return CD;
	}
	else if( strncmp(cmd, "kill", strlen("kill")) == 0) {
		return KILL;
	}
	return NO_SUCH_BUILTIN;
}

externalCommand( ParseInfo* parseInfo ) {
	int pid, status, returnCode, out, in, err, infile, outfile, errfile;
	struct commandType* com;
	
	com = &parseInfo->CommArray[0];	

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	err = dup(STDERR_FILENO);

	if( parseInfo->boolInfile == 1 ) {
		infile = open(parseInfo->inFile, O_RDONLY);
		if( infile < 0 ) {
			fprintf(stderr, "File cannot be read from");
		}
		else {
			dup2(infile, STDIN_FILENO);
		}
	}

	if( parseInfo->boolOutfile == 1 ) {
		outfile = open(parseInfo->outFile, O_WRONLY | O_CREAT | O_TRUNC );
		if( outfile < 0 ) {
			fprintf(stderr, "The file could not be opened for writing");
		}
		else {
			dup2(outfile, STDOUT_FILENO);
		}
	}

	if( parseInfo->boolErrfile == 1 ) {
	        errfile = open(parseInfo->errFile, O_WRONLY | O_CREAT | O_TRUNC );
       		if( outfile < 0 ) {
            		fprintf(stderr, "The file could not be opened for writing");
        	}
        	else {
            		dup2(errfile, STDERR_FILENO);
        	}
    	}

	if( parseInfo->boolAllOutfile == 1 ) {
       		outfile = open(parseInfo->allOutFile, O_WRONLY | O_CREAT | O_TRUNC );
        	if( outfile < 0 ) {
            		fprintf(stderr, "The file could not be opened for writing");
        	}
        	else {
            		dup2(outfile, STDOUT_FILENO);
			dup2(outfile, STDERR_FILENO);
        	}
    }

	if(parseInfo->boolBackground == 1){
		pid = fork();

		if( pid == 0 )
			execvp( com->command, com->varList);
		else
		{
			waitpid(0, &status, WNOHANG);
		}

		return;
	}

	pid = fork();
	if( pid == 0 )
	{
		returnCode = execvp(com->command, com->varList);
		if( returnCode == -1 )
		{
			printf("Not a valid command\n");
			exit(1);
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

	close(outfile);
	close(infile);
	close(errfile);

	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	dup2(err, STDERR_FILENO);
}

void changeDirectory( ParseInfo * info) {
	/* Grab hold of the directory */
	struct commandType  com = info->CommArray[0];

	/* Change to it */
	chdir( com.varList[1] );
}

void killJob( ParseInfo * info) {

}

int main(int argc, char **argv) {

	/* Variable Declaration */
	int pid, status;
	char * cmdLine; ParseInfo *info; /*info stores all the information returned by parser.*/
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
		/* Here we need to both maintain the history, and replace
			any commands of the form "!x" with the correct
			command */

		/* Code to replace the commands of the form "!x" */
		if( strncmp( cmdLine, "!", 1) == 0 )
		{/* Inside here means that the command is of the form "!x" */
			printf("Matched !x\n");
			if( strncmp( cmdLine, "!-1", 3) == 0 )
			{/* Inside here we just want the previous cmd */
				printf("Matched !-1\n");
				printf("cmdLine: %s\n", cmdLine);
			}
			else
			{/* Inside here we want the numbered cmd */

			}
		}
		else
		{
		printf("No match on !x\n");

		/* Update the history by shifting out commands */

		}

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
		else if( isBuiltInCommand(com->command) == JOBS) {
			returnCode = execvp(com->command, com->varList);
		}
		else if( isBuiltInCommand(com->command) == CD) {
			changeDirectory( info );
		}
		else if( isBuiltInCommand(com->command) == KILL) {
			killJob( info );
		}

		/*insert your code here.*/
		externalCommand(info);
		
		free_info(info);
		free(cmdLine);
	}/* while(1) */
}
