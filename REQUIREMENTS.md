
The prompt you print should indicate the current working directory. For example: 
The directory: /usr/foo/bar%
It may also indicate other things like machine name or username    or any other information you would like.
Try getcwd(char * buf, size_t size) .

You should allow the user to specify commands either by relative or absolute pathnames. To read in the command line, you may want to consider the readline function from the GNU readline library as it supports user editing of the command line.
Linux TIP! Example of a relative command df, same command with absolute path: /bin/df. To find the absolute path of a command use which, which ls gives /bin/ls

Try execvp it will search the path automatically for you. The first argument should be a pointer to the command string and the second argument should be a pointer to an array which contains the command string as arg[0] and the other arguments as arg[1] through arg[n].

You do not need to support setting of environment variables. However, you may find it useful to know about these variables especially PATH which is the list of directories in which to look for a specified executable. You may use execvp to have the system search the PATH inherited by your own shell from its parent.
Linux TIP! Use printenv PATH to see what's in your executable path

You should be able to redirect STDIN and STDOUT for the new processes by using < and >. For example, foo < infile > outfile would create a new process to run foo and assign STDIN for the new process to infile and STDOUT for the new process to outfile. In many real shells it gets much more complicated than this (e.g. >> to append, > to overwrite, >& redirect STDERR and STDOUT, etc.)! (WARNING: I am told that I/O redirection may be quite tricky on Windows. We may substitute a different feature here for Windows) You also do not have to support I/O redirection for built-in commands (it shouldn't be too hard but you don't have to do it.)
Note: one redirect in each direction is fine, not ls > foo < foo2

First open the file (use open or creat, open read only for infiles and creat writable for outfiles ) and then use dup2. 0 is the filedescriptor for STDIN and 1 is the file descriptor for STDOUT.
Examples: 
dup2 (fdFileYouOpened, fileno(stdin)) 
dup2 (fdFileYouOpened, fileno(stdout))

You should be able to place commands in the background with an & at the end of the command line. (You do not need to support moving processes between the foreground and the background (ex. bg and fg). You also do not need to support putting built-in commands in the background.)
Try waitpid(pid, status, options).

You should maintain a history of commands previously issued. The number of previous commands recorded can be a compile time constant of at least 10. This is a FIFO list, you should start numbering them with 1 and then when you exhaust your buffer you should discard the lowest number *BUT* keep incrementing the number of the next item. For example, if storing 10 commands, when the 11th is issued, you would be recording commands 2 through 11.
Linux TIP! The history command can show you the remembered commands by the linux shell, available with the up and down arrows

A user should be able to repeat a previously issued command by typing !number where number indicates which command to repeat. !-1 would mean to repeat the last command. !1 would mean repeat the command numbered 1 in the list of command returned by history.
Note: You can probably think of better syntax for this, but I thought it was good to stay as close as possible to syntax used by real shells

A built-in command is one for which no new process is created but instead the functionality is build directly into the shell itself. You should support the following built-in commands: jobs, cd, history, exit and kill.
jobs provide a numbered list of processes currently executing in the background.
Try waitpid with WNOHANG option to check without blocking. You can either check when jobs called or each time through the main while loop.

cd should change the working directory.
history should print the list of previously executed commands. The list of commands should include be numbered such that the numbers can be used with ! to indicate a command to repeat.
exit should terminate your shell process.
kill %num should terminate the process numbered in the list of background processes returned by jobs (by sending it a SIGKILL signal).
Note: Usually kill num refers to the process with ProcessId num; while kill %num refers to the process in the jobs list with number num

Try kill (pid, SIGKILL) .

I would also *highly* recommend the built-in function help that lists the available built-in commands and their syntax. (If you don't follow the syntax expected, then a help function would let the graders proceed anyway.)
If the user chooses to exit while there are background processes, notify the user that these background processes exist, do not exit and return to the command prompt. The user must kill the background processes before exiting.
You may assume that each item in the command string is separated on either side by at least one space (e.g. prog > outfile rather than prog>outfile).