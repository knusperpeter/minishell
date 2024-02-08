NAME = pipex
CC		= cc
AR		= ar
RM		= rm
CFLAGS	= -Wall -Wextra -Werror -g
HDRS	= include/pipex.h

PRINTF		= printf
PRINTF_A	= ${PRINTF}/libftprintf.a

SOURCES	=	pipex.c \
			init.c \
			inout_open.c \
			cleanup.c \
			parse.c \
			exec.c \
			gnl/get_next_line_utils.c\
			gnl/get_next_line.c


OBJECTS = $(SOURCES:.c=.o)

all		: $(NAME)

$(NAME)	: $(OBJECTS) $(HDRS)
	make --no-print-directory all -C $(PRINTF)
	$(CC) $(SOURCES) $(CFLAGS) -o $(NAME) -L. ${PRINTF_A}


%.o		: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	make --no-print-directory clean -C $(PRINTF)
	$(RM) -rf $(OBJECTS)

fclean	:
	make --no-print-directory fclean -C $(PRINTF)
	$(RM) -rf $(OBJECTS)
	$(RM) -rf $(NAME)

re		: fclean all

# .SILENT:
.PHONY: all clean fclean re