# Minishell

## Summary

Minishell is a simple shell, similar to bash, written in C. It provides a command line interface for users to interact with the system. This project is a great way to learn about processes and file descriptors.

## Features

- Displays a prompt when waiting for a new command.
- Maintains a working history.
- Searches and launches the right executable (based on the PATH variable or using a relative or absolute path).
- Handles single and double quotes.
- Implements redirections (`<`, `>`, `<<`, `>>`).
- Implements pipes (`|`).
- Handles environment variables (`$`).
- Handles `$?` which expands to the exit status of the most recently executed foreground pipeline.
- Handles ctrl-C, ctrl-D and ctrl-\ signals.
- Implements the following builtins: echo (with option -n), cd, pwd, export, unset, env, exit.

## Building and Running

The project includes a Makefile for building the executable. Use the following commands:

- To build the project, run `make`.
- To clean the build, run `make clean`.
- To run the shell, use `./minishell`.

## Notes

- The readline() function can cause memory leaks. Create a suppression file if you want to look for leaks.
