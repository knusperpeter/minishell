NAME = minishell
CC		= cc
AR		= ar
RM		= rm
CFLAGS	= -Wall -Wextra -Werror -Ofast -march=native -g
LFLAGS	= -lreadline
HDRS	= include/minishell.h
LIBFT	= libft
LIBFT_LIB	= libft.a

SOURCES	=	src/minishell.c \
			src/init_env.c \
			src/free.c \
			src/parsing_cmd_table.c \
			src/open_io.c \
			src/exec.c \
			src/builtins/ft_echo.c	\
			src/builtins/ft_env.c \
			src/builtins/ft_exit.c \
			src/builtins/ft_export.c \
			src/builtins/ft_pwd.c \
			src/builtins/ft_unset.c \
			src/builtins/ft_cd_1.c \
			src/builtins/ft_cd_2.c

OBJECTS = $(SOURCES:.c=.o)

all		: $(NAME)

$(NAME)	: $(OBJECTS) $(HDRS)
	make --no-print-directory all -C $(LIBFT)
	$(CC) $(SOURCES) $(CFLAGS) $(LFLAGS) -o $(NAME) -L$(LIBFT) -lft

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