# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caigner <caigner@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 11:49:47 by miheider          #+#    #+#              #
#    Updated: 2024/04/22 17:24:29 by caigner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# M A K E F I L E

NAME=libft.a

SRCS=ft_isprint.c ft_isalpha.c ft_isdigit.c ft_strjoin.c ft_strlen.c \
	 ft_strrchr.c ft_toupper.c ft_strchr.c ft_isalnum.c ft_isascii.c \
	 ft_tolower.c ft_bzero.c ft_memset.c ft_memcpy.c ft_memmove.c ft_calloc.c \
	 ft_memchr.c ft_strdup.c ft_strlcpy.c ft_strlcat.c ft_memcmp.c ft_strncmp.c \
	 ft_strnstr.c ft_atoi.c ft_strtrim.c ft_putchar_fd.c ft_putstr_fd.c \
	 ft_putendl_fd.c ft_putnbr_fd.c ft_split.c ft_striteri.c ft_strmapi.c \
	 ft_itoa.c ft_substr.c ft_atoll.c ft_lltoa.c \
	 get_next_line_heredoc.c get_next_line.c ft_lstnew.c ft_lstsize.c \
	 ft_lstadd_front.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c \
	 ft_lstiter.c

OBJS=$(SRCS:.c=.o)

CC=cc
CFLAGS=-Wall -Wextra -Werror -g
RM=rm -rf 

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) -r $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(B_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean, fclean, re
