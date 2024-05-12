/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:38:11 by caigner           #+#    #+#             */
/*   Updated: 2024/05/13 01:10:40 by caigner          ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define MESSAGE1 "❌ minishell: syntax error near unexpected token `<<<'\n"
# define MESSAGE2 "❌ minishell: syntax error near unexpected token `>>>'\n"
# define MESSAGE3 "❌ minishell: syntax error near unexpected token `<<'\n"
# define MESSAGE4 "❌ minishell: syntax error near unexpected token `>>'\n"
# define MESSAGE5 "❌ minishell: syntax error near unexpected token `<'\n"
# define MESSAGE6 "❌ minishell: syntax error near unexpected token `>'\n"
# define MESSAGE7 "❌ minishell: syntax error near unexpected token `newline'\n"
# define MESSAGE8 "minishell: .: filename argument required\n.: usage: . filename [arguments]\n"
# define MESSAGE9 "❌ minishell: syntax error near unexpected token `"
# define MESSAGE10 "❌ minishell: syntax error near unexpected token `|'\n"
# define MESSAGE11 "❌ minishell: syntax error near unexpected token `||'\n"

extern int	g_signal;

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
	int				expand_heredoc;
}	t_io_red;

// Inputs and what attributes come with them is locatedhere.
typedef struct s_cmd_table
{
	pid_t				id;
	int					read_fd;
	int					write_fd;
	t_list				*io_red;
	t_list				*cmds;
	t_io_red			*in;
	t_io_red			*out;
	char				*heredoc_name;
	char				**str;
	char				*exec_path;
}	t_cmd_table;

// Common struct
typedef struct common_data
{
	pid_t				pid;
	int					cmd_count;
	int					open_single_quotes;
	int					open_double_quotes;
	int					subshell_level;
	int					heredoc_counter;
	t_env				*env;
	char				**envp;
	t_list_d			*cmd_struct;
	t_list				*tokens;
	unsigned int		exitstatus;
	// char				*exitstatus_str;
	char				*raw_prompt;
	int					old_pipe;
}	t_common;

//main
int			ft_loop(t_common *c);
//exec
void		wait_all_childs(t_common *c);
int			is_builtin(char *cmd);
char		**get_envp(t_common *c, t_env *env);
void		close_fds(t_common *c, int *fd, t_cmd_table *cmd);
int			ft_count_cmds(t_list_d *cmd_struct);
int			ft_builtins(t_cmd_table *cmd, t_common *c);
void		ft_redirect_io(t_common *c, t_cmd_table *cmd, int curr, int *fd);
void		handle_fds_parent(t_common *c, int *fd);
int			ft_execute(t_common *c);
//init_env
int			create_list_element(t_common *c, void **element, size_t size);
int			ft_init_env(t_common *c, t_env *node, char *envp, t_env *prev);
int			dup_env(t_common *c, char **envp);
//free
void		free_2d(char **str);
void		free_tokens(void *content);
void		free_io_red(void *content);
void		free_env_nodes(t_env *start);
void		free_cmd_table(void *content);
void		ft_cleanup_loop(t_common *c);
void		free_all(t_common *c, int cleanup_loop);
void		ft_clean_exit(t_common *c, char *msg, int cleanup_loop);
//open_io
int			open_io(t_common *c, t_list *io, t_cmd_table *cmd_node);
void		open_failed(t_io_red *io, char *file);
void		unlink_heredoc(t_io_red *io, t_cmd_table *cmd);
void		ft_close_old_fd(t_cmd_table *cmd_node, t_io_red *io);
void		here_doc(t_common *c, t_io_red *io, int *fd);
int			open_redirections(t_common *c, t_cmd_table *cmd_node);
//utils
t_list_d	*ft_lstnew_d(void *content);
t_list_d	*ft_lstlast_d(t_list_d *lst);
void		ft_lst_d_add_back(t_list_d **lst, t_list_d *neu);
void		ft_lst_d_delone(t_list_d *lst, void (*del)(void *));
void		ft_lst_d_clear(t_list_d **lst, void (*del)(void *));
void		ft_printerrno(char *s);
int			get_env_size(t_env *env);
void		*ft_protect(t_common *c, void *p1, void *p2, void *p3, void *p4);
//expansion & quotes
void		ft_rm_quotes(t_common *c, t_list_d *cmds);
char		*expand_str(t_common *c, char *str);
int			check_split(t_common *c, t_cmd_table **t, t_list **a, t_list *b);
void		ft_expansion(t_common *c, t_list_d *cmds);
char		*get_expansion_value(t_common *c, char *str, int i, int *varsize);
char		*get_expanded_str(t_common *c, char *str, int i, int varsize);
void		heredoc_expansion(t_common *c, t_io_red *io, char **str);
int			has_expansion(t_common *c, char *str);
void		handle_quote_state(t_common *common, char c);
//get_cmd_path
int			is_dir(char *file);
char		**ft_get_paths(t_common *c, t_env *env);
char		*join_path(t_common *c, char *cmd, char *path);
int			get_cmd_path(t_common *c, t_cmd_table *cmd);
//lexer_1
int			check_token(char *token);
int			check_char(char *character);
int			is_delim(char c, const char *delim);
char		*handle_quote(char *str, int *in_quotes, char *quote_type);
char		*handle_single_quote(char *str, int *in_quotes, char *quote_type);
//lexer_2
char		*quotes_in_strtok(char *str, const char *delim);
char		*ft_strtok(char *s1, const char *delim);
int			create_token(t_token **token, char *data);
int			check_and_create_token(char **value, int i, t_token **token);
int			add_token(t_token **lst, char **value, int i, t_token **tmp);
//lexer_3.c
void		init_add_to_list(int *index, t_token **tmp, t_token **last);
void		add_to_list(char **token, t_list *lst);
int			check_tokens(char *input);
char		**tokenize_input(char *input);
int			process_special_character(int *i, int *j, char *input, char *new_string);
//lexer_4.c
int			handle_non_special_character(int *i, int *j, char *input, char *new_string);
int			no_space_array(int *i, int *j, char *input, char *new_string);
char		*allocate_memory(int size);
char		**set_up_array(int cc, char *input);
void		count_up(int *i, int *cc);
//lexer_5.c
void		prep_input_three(int i, int *cc, char *input);
int			prep_input_two(int *i, int *cc, char *input);
void		prep_input_one(int i, int *cc, char *input);
char		**prep_input(char *input);
int			error_and_cleanup(t_common *c, int status);
//lexer_6.c
int			check_this(t_common *c, char *result, int j);
int			check_dot(t_common *c, char *result, int k, int j);
int			check_quotes(t_common *c, char *result, int k);
int			condition_and_error(t_common *c, char *result, int k, int status);
int			check_again(t_common *c, char *result, int k, char fir);
//lexer_7.c
int			check_more_cont(t_common *c, char *result, int k, int status);
int			check_more(t_common *c, char *result, int k, char fir);
int			check_that(t_common *c, char *result, int k);
int			check_condition_and_error(char *result, int *i);
int			check_one_more(t_common *c, char *result);
//lexer_8.c
void		init_for_open_quotes(int *s_quote, int *d_quote, int *i);
int			open_quotes(t_common *c, char *result);
int			check_sq(t_common *c, char *result, int k);
int			check_following(t_common *c, char *result, int k, char fir);
void		init_initial_checks(int *i, int *k, int*len, char *result);
//lexer_9.c
int			initial_checks(t_common *c, char *result);
int			error_check_pipes(t_common *c, int *i, int *pipe, char *input);
char		**tokenize_one(t_common *c, char *input, int pipe);
int			check_empty_line(t_common *c, char *input, int pipe);
int			check_result(t_common *c, char *result);
//lexer.c
int			is_in_quotes(char *str, int i);
int			ignore_pipe(char *str, int i);
void		count_pipes_cont(t_common *c, char *input, int *i, int *pipe);
int			count_pipes(t_common *c, char *input);

int			error_lexer(t_common *c, char *s, int i);
int			tokenize(t_common *c);


//parsing
int			ft_parsing(t_common *c);
void		ft_cmd_args_to_2d(t_common *c, t_list_d *cmd_table);
int			red_to_node(t_common *c, t_token *token, t_cmd_table *node);
void		init_cmd_table(t_cmd_table *node);
//signals
void		interactive(t_common *c);
void		interactive_here(t_common *c);
void		non_interactive(t_common *c);
void		cmd_bs(int sig);

// builtins
int			ft_pwd(t_common *c, char **args);
int			ft_env(char **args, t_env *env);
int			ft_unset(char **args, t_common *c);
void		ft_exit(t_common *c, char **cmd);
int			is_valid_env(char *env);
void		export_print_env(t_env *env);
void		add_to_var(t_common *c, t_env *env, char *arg);
int			ft_export(t_common *c, char **args, t_env *env);
int			ft_echo(t_common *c, char **args);
void		set_env_value(t_env *env, char *variable, char *value);
int			ft_cd(char **args, t_common *c);
int			get_set_path(t_env *env, char *variable, char **path);

//int	ft_execute_builtins(t_cmd_table *cmd, t_common *c);

#endif
