**Minishell** is a project that aims to create a simple shell, similar to bash, using the C programming language. The shell is designed to communicate with the computer using interactive lines of commands in a language somewhat close to human language.

The end program, named "minishell", is capable of displaying a prompt when waiting for a new command, maintaining a working history, and searching and launching the correct executable based on the PATH variable or using a relative or absolute path. It handles single and double quotes, implements redirections and pipes, and handles environment variables and certain special signals like ctrl-C, ctrl-D and ctrl-.

The shell also implements several built-in commands like echo, cd, pwd, export, unset, env, and exit. The program is written in accordance with the Norm and is expected to handle errors gracefully without unexpected quits.

The project is evaluated based on the code submitted to the assigned git repository and is expected to be free of memory leaks and other malfunctions.