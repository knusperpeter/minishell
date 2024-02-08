NAME = libftprintf.a
LIBFT = ../libft
OBJECTS = $(SOURCES:.c=.o)
CC = cc
AR = ar
RM = rm
CFLAGS = -Wall -Werror -Wextra

SOURCES = ft_printf.c printf_functions.c printf_functions2.c

# COLOUR_GREEN = '\033[0;32m'
# COLOUR_RED = '\033[0;31m'
# COLOUR_BLUE = '\033[0;34m'
# END_COLOUR = '\033[0m'

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(LIBFT)
	cp ${LIBFT}/libft.a .
	mv libft.a $(NAME)
	$(AR) -rcs $(NAME) $(OBJECTS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean --no-print-directory -C $(LIBFT)
	$(RM) -rf $(OBJECTS)

fclean:
	make fclean --no-print-directory -C $(LIBFT)
	$(RM) -rf $(OBJECTS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
