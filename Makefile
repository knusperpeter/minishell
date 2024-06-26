# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caigner <caigner@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/10 20:04:47 by miheider          #+#    #+#              #
#    Updated: 2024/05/19 13:41:33 by caigner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC		= cc
AR		= ar
RM		= rm
CFLAGS	= -Wall -Wextra -Werror -Wno-unused-result -march=native -g
LFLAGS	= -lreadline
HDRS	= include/minishell.h
LIBFT	= libft
LIBFT_LIB	= libft.a

SOURCES	=	src/builtins/ft_echo.c \
			src/builtins/ft_env.c \
			src/builtins/ft_exit.c \
			src/builtins/ft_export_1.c \
			src/builtins/ft_export_2.c \
			src/builtins/ft_pwd.c \
			src/builtins/ft_unset.c \
			src/builtins/ft_cd_1.c \
			src/builtins/ft_cd_2.c \
			src/minishell.c \
			src/init_env.c \
			src/lexer.c \
			src/lexer_prep_input.c \
			src/lexer_pipe.c \
			src/lexer_error_tree_one.c \
			src/lexer_error_tree_two.c \
			src/lexer_set_up_array.c \
			src/lexer_utils.c \
			src/lexer_tokenizing.c \
			src/lexer_token.c \
			src/parsing_1.c \
			src/parsing_2.c \
			src/parsing_3.c \
			src/open_io_1.c \
			src/open_io_2.c \
			src/get_cmd_path_1.c \
			src/get_cmd_path_2.c \
			src/exec_1.c \
			src/exec_2.c \
			src/exec_3.c \
			src/utils_1.c \
			src/utils_2.c \
			src/free_1.c \
			src/free_2.c \
			src/lexer_error.c \
			src/signals.c \
			src/signal_2.c \
			src/expansion_1.c \
			src/expansion_2.c \
			src/expansion_3.c \
			src/remove_quotes.c \
			src/expansion_heredoc.c

OBJECTS = $(SOURCES:.c=.o)

all		: $(NAME)

$(NAME)	: $(OBJECTS) $(HDRS)
	make --no-print-directory all -C $(LIBFT)
	$(CC) $(SOURCES) $(CFLAGS) $(LFLAGS) -o $(NAME) -L$(LIBFT) -lft
	@if [ $$? -eq 0 ]; then echo "\033[1;32mminishell [OK]\033[0m"; else echo "\033[1;31mminishell [KO]\033[0m"; fi

%.o		: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	make --no-print-directory clean -C $(LIBFT)
	$(RM) -rf $(OBJECTS)

fclean	:
	make --no-print-directory fclean -C $(LIBFT)
	$(RM) -rf $(OBJECTS)
	$(RM) -rf $(NAME)

re		: fclean all

# .SILENT:
.PHONY: all clean fclean re
