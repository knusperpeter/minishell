/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:56 by caigner           #+#    #+#             */
/*   Updated: 2024/02/21 14:50:38 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <limits.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <sys/wait.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

// enum can be used anywhere. VOID == 0, PIPE == 1, STRING == 2, ...
// https://www.programiz.com/c-programming/c-enumeration
// Ex.: "common->simple_cmd->type = VOID" is the same as
//		"common->simple_cmd->type = 0"
typedef enum e_type
{
	VOID,
	PIPE,
	STRING,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
}	t_type;

typedef struct s_list_d
{
	void			*content;
	struct s_list_d	*prev;
	struct s_list_d	*next;
}	t_list_d;

// Environment key-value pairs are saved here. The flag indicates if
// the key is defined or not.
typedef struct s_env
{
	char			*variable;
	char			*value;
	int				flag;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_token // "< in" -> type = REDIR_IN     data = "in"
{
	t_type			type;
	char			*data;
	s_token			*next;
}	t_token;

// Inputs and what attributes come with them is locatedhere.
// Each pipe stands for a new node.???????
typedef struct s_cmd_table
{
	int					read_fd;
	int					write_fd;
	int					io_red;
	char				**str;
	char				*heredoc_name;
	struct s_cmd_table	*next;//not needed i guess?
	struct s_cmd_table	*prev;//
}	t_cmd_table;

typedef struct s_final_cmd_table //do i need this? no, exec to cmd_table
{
	char			**simple_cmd;
	char			*exec_path;
	char			**env;
	int				read_fd;
	int				write_fd;
}	t_final_cmd_table;

// Common struct
typedef struct common_data
{
	t_env				*env;
	t_list				*cmd_struct;
	t_list				*tokens; //t_token
	unsigned int		exitstatus;
	char				*raw_prompt;
	t_final_cmd_table	*final_cmd;
}	t_common;

int		create_list_element(void **element, size_t size);
int		ft_init_env(t_env *node, char *envp, t_env *prev);
int		dup_env(t_common *c, char **envp);
void	free_2d(char **str);
void	free_env_nodes(t_env *start);
void	free_cmd_table(void *content);
void	free_all(t_common *c, t_cmd_table *cmds);
int		ft_exec(t_common *c);

// builtins
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_unset(char **args, t_common *c);
void	ft_exit(t_common *c, char **cmd);
int		ft_export(char **args, t_env *env);
int		ft_echo(char **args);
void	set_env_value(t_env *env, char *variable, char *value);
int		ft_cd(char **args, t_common *c);

#endif