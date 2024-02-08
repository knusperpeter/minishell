/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:56 by caigner           #+#    #+#             */
/*   Updated: 2024/02/08 13:49:13 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
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
// Ex.: "common->tokenslist->type = VOID" is the same as
//		"common->tokenslist->type = 0"
typedef enum e_type
{
	VOID,
	PIPE,
	STRING,
	REDIR_L,
	REDIR_R,
	APPEND,
	HEREDOC_L,
}	t_type;

//Promptdata. Self-explainatory...
typedef struct PromptData
{
	char			*username;
	char			*hostname;
	char			*prompt_text;
}	t_prompt;

//Environment key-value pairs are saved here. The flag indicates if
//the key is defined or not.
typedef struct s_env
{
	char			*variable;
	char			*value;
	int				flag;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

//Inputs and what attributes come with them is locatedhere.
//Each pipe stands for a new node.???????
struct s_node
{
	char			**str;
	int				quote;
	int				in_fd;
	int				out_fd;
	int				builtin;
	t_type			type;
	struct s_node	*next;
	struct s_node	*prev;
};

//Common struct
typedef struct common_data
{
	t_prompt		*prompt;
	t_env			*env;
	struct s_node	*tokenslist;
	unsigned int	exitstatus;
	char			*raw_prompt;
	
	struct termios	old_settings;
	struct termios	settings;
}	t_common;

int	create_list_element(void **element, size_t size);

//builtins
int	ft_pwd(void);
int	ft_env(t_env *env);
void	ft_exit(t_common *c, char **cmd);
int	ft_export(char **args, t_env *env);
void	free_env_nodes(t_env *start);
int	dup_env(t_common *c, char **envp);
int	ft_init_env(t_env *node, char *envp, t_env *prev);

#endif