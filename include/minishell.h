/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:38:11 by caigner           #+#    #+#             */
/*   Updated: 2024/05/19 00:18:28 by caigner          ###   ########.fr       */
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
# define MESSAGE1 "minishell: syntax error near unexpected token `<<<'\n"
# define MESSAGE2 "minishell: syntax error near unexpected token `>>>'\n"
# define MESSAGE3 "minishell: syntax error near unexpected token `<<'\n"
# define MESSAGE4 "minishell: syntax error near unexpected token `>>'\n"
# define MESSAGE5 "minishell: syntax error near unexpected token `<'\n"
# define MESSAGE6 "minishell: syntax error near unexpected token `>'\n"
# define MESSAGE7 "minishell: syntax error near unexpected token `newline'\n"
# define MESSAGE8 "minishell: .: filename argument required\n\
.: usage: . filename [arguments]\n"
# define MESSAGE9 "minishell: syntax error near unexpected token `"
# define MESSAGE10 "minishell: syntax error near unexpected token `|'\n"
# define MESSAGE11 "minishell: syntax error near unexpected token `||'\n"
# define MESSAGE12 "minishell: syntax error: dquote\n"
# define MESSAGE13 ": command not found\n"

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
	int					permission;
	int					read_fd;
	int					write_fd;
	t_list				*io_red;
	t_list				*cmds;
	t_io_red			*in;
	t_io_red			*out;
	char				*hd_name;
	char				**str;
	char				*exec_path;
}	t_cmd_table;

// Common struct
typedef struct common_data
{
	int					cmd_count;
	int					open_single_quotes;
	int					open_double_quotes;
	int					subshell_level;
	int					heredoc_counter;
	t_env				*env;
	t_env				*hidden_env;
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
void		open_failed(t_common *c, t_io_red *io, char *file, \
			t_cmd_table *cmd);
void		unlink_heredoc(t_io_red *io, t_cmd_table *cmd);
void		ft_close_old_fd(t_cmd_table *cmd_node, t_io_red *io);
int			here_doc(t_common *c, t_io_red *io, int *fd);
int			open_redirections(t_common *c, t_cmd_table *cmd_node);
void		dir_error(t_common *c, char *file);
//utils
t_list_d	*ft_lstnew_d(void *content);
t_list_d	*ft_lstlast_d(t_list_d *lst);
void		ft_lst_d_add_back(t_list_d **lst, t_list_d *neu);
void		ft_lst_d_delone(t_list_d *lst, void (*del)(void *));
void		ft_lst_d_clear(t_list_d **lst, void (*del)(void *));
void		ft_printerrno(char *s);
int			get_env_size(t_env *env);
void		*ft_protect(t_common *c, void *p1, void *p2, void *p3);
void		cmd_not_found(char *cmd);
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
//lexer_prep_input
void		update_counts(int *i, int *cc, char *input, int num);
void		process_checks(char *input, int *i, int *cc, int len);
void		init_prep_input(int *i, int *cc, int *len, char *input);
int			check_the_end(char *input, int i, int *cc);
char		**prep_input(t_common *c, char *input);
//lexer_pipe
int			skip_whitespace(char *input, int i, int dir);
int			check_pipe_error_last(t_common *c, char *input, int len);
int			handle_i_and_pipe(char *input, int i, int *pipe);
int			check_pipe_error(t_common *c, char *input, int len);
int			count_pipes(t_common *c, char *input);
//lexer_error_tree_one.c
void		es_cul(t_common *c, int es);
int			analysis_block_redir(t_common *c, char *input, int i, int count);
int			check_block_redir(t_common *c, char *input);
int			check_last_redir(t_common *c, char *input);
int			check_dots(t_common *c, char *input, int i, int k);
//lexer_error_tree_two.c
int			check_empty_quotes(t_common *c, char *input);
int			check_open_quotes(t_common *c, char *result);
int			check_next(char *input, int i);
int			check_invalid_redir(t_common *c, char *input);
int			error_tree(t_common *c, char *input);
//lexer_set_up_array
char		**set_up_array(t_common *c, int cc, char *input);
void		process_input(char *input, char *new_string, int cc);
void		process_character(char *input, int *k, char *new_string, int *j);
void		handle_special_characters(char *input, int *k, char *new_string, \
			int *j);
int			handle_whitespace(char *input, int k, char *new_string, int *j);
//lexer_utils.c
int			skip_whitespace(char *input, int i, int dir);
int			q_status(char *input, int i);
int			check_the_char(char *input, int i, int len);
int			check_space_before(char *input, int k);
int			check_space_after(char *input, int k);
//lexer_tokenizing.c
int			check_tokens(char *input);
int			check_double(char *input, int i, int len);
char		**tokenize_input(char *input);
void		counting_up(int *i, int *cc, int a, int b);
char		**tokenize_one(t_common *c, char *input, int pipe);
//lexer_token.c
void		count_up(int *i, int *cc);
int			add_token(t_common *c, char **value, int i, t_token **tmp);
void		init_add_to_list(t_token **last, t_token **tmp, int *index);
void		add_to_list(t_common *c, char **token, t_list *lst);
//lexer.c
int			check_token(char *token);
int			is_delim(char c, const char *delim);
char		*quotes_in_strtok(char *str, const char *delim, int in_quotes, \
			char quote_type);
char		*ft_strtok(char *s1, const char *delim);
//lexer_error.c
int			error_lexer(t_common *c, int i);
//parsing
int			ft_parsing(t_common *c);
void		ft_cmd_args_to_2d(t_common *c, t_list_d *cmd_table);
int			red_to_node(t_common *c, t_token *token, t_cmd_table *node);
void		init_cmd_table(t_cmd_table *node);
int			tokenize(t_common *c);
//signals.c
void		cmd_c_ia(int sig);
void		cmd_c_nonia(int sig);
void		cmd_bs(int sig);
void		cmd_c_here(int sig);
//signals_2.c
void		interactive(t_common *c);
void		interactive_here(t_common *c);
void		non_interactive(t_common *c);

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
int			get_path(char **args, char **oldpwd, char **path, t_common *c);
void		join_path_else(t_common *c, char **path, char *oldpwd, char *args);
int			ft_cd(char **args, t_common *c);
int			get_set_path(t_env *env, char *variable, char **path);

//int	ft_execute_builtins(t_cmd_table *cmd, t_common *c);

#endif
