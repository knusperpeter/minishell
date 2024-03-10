NAME = minishell
CC		= cc
AR		= ar
RM		= rm
CFLAGS	= -Wall -Wextra -Werror -Wno-unused-result -Ofast -march=native -g
LFLAGS	= -lreadline
HDRS	= include/minishell.h
LIBFT	= libft
LIBFT_LIB	= libft.a

SOURCES	=	src/builtins/ft_echo.c	\
			src/builtins/ft_env.c \
			src/builtins/ft_exit.c \
			src/builtins/ft_export.c \
			src/builtins/ft_pwd.c \
			src/builtins/ft_unset.c \
			src/builtins/ft_cd_1.c \
			src/builtins/ft_cd_2.c \
			src/minishell.c \
			src/init_env.c \
			src/lexer_new.c \
			src/lexer_utils.c \
			src/parsing_cmd_table.c \
			src/expansion.c \
			src/open_io.c \
			src/get_cmd_path.c \
			src/exec.c \
			src/utils_1.c \
			src/free.c \
			src/lexer_error.c

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
