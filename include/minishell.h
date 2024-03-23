/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:56 by caigner           #+#    #+#             */
/*   Updated: 2024/03/23 19:29:36 by caigner          ###   ########.fr       */
=======
<<<<<<< HEAD
/*   By: miheider <miheider@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:56 by caigner           #+#    #+#             */
/*   Updated: 2024/03/23 15:01:40 by miheider         ###   ########.fr       */
=======
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:49:56 by caigner           #+#    #+#             */
/*   Updated: 2024/03/23 16:34:20 by caigner          ###   ########.fr       */
>>>>>>> main
>>>>>>> main
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
# include <signal.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

//volatile	g_signal = 0;

typedef enum e_type
{
	VOID,
//	STRING,
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
	struct s_token	*next;
}	t_token;

typedef struct s_io_red
{
	t_type			type;
	char			*infile;
	char			*outfile;
	char			*heredoc_limiter;
}	t_io_red;

// Inputs and what attributes come with them is locatedhere.
typedef struct s_cmd_table
{
	pid_t				id;
	int					read_fd;
	int					write_fd;
	t_list				*io_red;
	t_list				*cmds;
	char				*heredoc_name;
	char				**str;
	char				*exec_path;
}	t_cmd_table;

// Common struct
typedef struct common_data
{
	pid_t				pid;
	int					cmd_count;
	int					subshell_level;
	t_env				*env;
	char				**envp;
	t_list_d			*cmd_struct;
	t_list				*tokens;
	unsigned int		exitstatus;
	char				*exitstatus_str;
	char				*raw_prompt;
	int					old_pipe;
}	t_common;


int		create_list_element(void **element, size_t size);
int		ft_init_env(t_env *node, char *envp, t_env *prev);
int		dup_env(t_common *c, char **envp);
void	free_2d(char **str);
void	free_env_nodes(t_env *start);
void	free_cmd_table(void *content);
void	ft_cleanup_loop(t_common *c);
void	free_all(t_common *c, int cleanup_loop);
void	ft_clean_exit(t_common *c, char *msg, int cleanup_loop);
int		ft_loop(t_common *c);
int		ft_parsing(t_common *c);
int		open_io(t_list *io, t_cmd_table *cmd_node);
int		ft_execute(t_common *c);
void	ft_printerrno(char *s);
char	**tokenize_one(char *input, int pipe);
char    **prep_input(char *input);
void	add_to_list(char **token, t_list *lst);
t_list_d	*ft_lstnew_d(void *content);
t_list_d	*ft_lstlast_d(t_list_d *lst);
void	ft_lst_d_add_back(t_list_d **lst, t_list_d *neu);
void	ft_lst_d_delone(t_list_d *lst, void (*del)(void *));
void	ft_lst_d_clear(t_list_d **lst, void (*del)(void *));
void	ft_rm_quotes(t_list_d *cmds);

//lexer
void    error_lexer(char *s, int i);
int		get_cmd_path(t_common *c, t_cmd_table *cmd);
void	ft_expansion(t_common *c, t_list_d *cmds);
int		count_pipes(char *input);
char	**set_up_array(int cc, char *input);
char	**prep_input(char *input);
char	*ft_strtok(char *s1, const char *delim);
int		check_char(char *character);
int		check_token(char *token);

//signals
void    interactive(void);

// builtins
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_unset(char **args, t_common *c);
void	ft_exit(t_common *c, char **cmd);
int		ft_export(char **args, t_env *env);
int		ft_echo(char **args);
void	set_env_value(t_env *env, char *variable, char *value);
int		ft_cd(char **args, t_common *c);

//int	ft_execute_builtins(t_cmd_table *cmd, t_common *c);

#endif
