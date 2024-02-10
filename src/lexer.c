/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/02/06 21:14:34 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include "../include/minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	x;
	unsigned int	y;

	x = 0;
	while (src[x] != '\0')
		x++;
	if (size == 0)
		return (x);
	y = 0;
	while (src[y] != '\0' && y < (size -1))
	{
		dst[y] = src[y];
		y++;
	}
	dst[y] = '\0';
	return (x);
}

int check_string(char input, char next)
{
    char special[11];
    char *special_ptr = special;

	printf("%c\n", input);
    ft_strlcpy(special, "*?[()]<>|#", 10);
	if (input == 34 || input == 39 || input == 46)
		return (3);
    while (*special_ptr)
    {
        if (*special_ptr == input && input == next)
            return (2);
		else if (*special_ptr == input && next == '\0')
			return (4);
        else if (*special_ptr == input)
            return (1);
        else
            special_ptr++;
    }
    return (0);
}

void    ft_counter(long unsigned int *cc, char **input, long unsigned int *wc, int x)
{
    if (x == 0)
    {
        *cc += 1;
        (*input)++;
    }
    if (x == 1)
    {
        *cc += 1;
        (*input)++;
        *wc += 1;
    }
    if (x == 2)
    {
        *cc += 1;
        (*input)++;
        *wc += 2;
    }
    if (x == 3)
    {
        *cc += 2;
        (*input) += 2;
        *wc += 1;
    }
    if (x == 4)
    {
        *cc += 2;
        (*input) += 2;
    }
    if (x == 5)
    {
        *cc += 2;
        (*input) += 2;
        *wc += 2;
    }
}


void	preparing_input(char *input, char first)
{
	size_t	wc;				//word counter (zählt alle Wörter)
	size_t	cc;				//char-count (zählt alle nicht-spaces)

	cc = 0;
	wc = 0;
	while (*input)
	{
		while (*input && *input == 32)
		{
			input++;
		}
		if (*input != 32)
		{
			if (*input == '\0')
				break;
			wc++;
		}
		while (*input && *input != ' ')
		{
			if (*input && *input == 46) // . (dot) instead of " and '
            {
				if (*(input - 1) == 32)
				    ft_counter(&cc, &input, &wc, 0);
				else if (*(input - 1) != 32)
				    ft_counter(&cc, &input, &wc, 1);
                while (*input && *input != 46)
				    ft_counter(&cc, &input, &wc, 0);
                if (*input && *input == 46 && *(input + 1) == 32)
				    ft_counter(&cc, &input, &wc, 0);
				else if (*input && *input == 46 && *(input + 1) != 32)
				    ft_counter(&cc, &input, &wc, 1);
            }
			else if (check_string(*input, *(input + 1)) == 0)
				ft_counter(&cc, &input, &wc, 0);
			else if (check_string(*input, *(input + 1)) == 3)
			{
				ft_counter(&cc, &input, &wc, 0);
				while (*input != 34 || *input != 39)
				    ft_counter(&cc, &input, &wc, 0);
				ft_counter(&cc, &input, &wc, 1);
			}
			else if (check_string(*input, *(input + 1)) == 1)
			{
				if (*(input - 1) == 32 && (*(input + 1) != 32 || *(input + 1) == '\0'))
				    ft_counter(&cc, &input, &wc, 1);
				else if (*(input - 1) != 32 && ((*(input + 1) == 32) || *(input + 1) == '\0'))
				    ft_counter(&cc, &input, &wc, 1);
				else if (*(input - 1) == 32 && *(input + 1) == 32)
				    ft_counter(&cc, &input, &wc, 0);
				else
				    ft_counter(&cc, &input, &wc, 2);
				if (*(input - 2) == '.' || *input == '.')   //. insteadt of ' or
					wc--;
			}
			else if (check_string(*input, *(input + 1)) == 2)
			{
				if (*(input - 1) == 32 && (*(input + 2) != 32 || *(input + 2) == '\0'))
				    ft_counter(&cc, &input, &wc, 3);
				else if (*(input -1) != 32 && (*(input + 2) == 32 || *(input + 2) == '\0'))
				    ft_counter(&cc, &input, &wc, 3);
				else if (*(input - 1) == 32 && *(input + 2) == 32)
				    ft_counter(&cc, &input, &wc, 4);
				else
				    ft_counter(&cc, &input, &wc, 5);
				if (*input == '\0' || *(input - 3) == '.' || *input == '.') // . instead of ' or "
					wc--;
			}
			else if (check_string(*input, *(input + 1)) == 4)
			{
				if (/**input &&*/ *(input - 1) != ' ')
				    ft_counter(&cc, &input, &wc, 1);
				else if (/**input &&*/ *(input - 1) == ' ')
				    ft_counter(&cc, &input, &wc, 0);
			}
		}
	}
	if (first == '|')								//add all special characters
		wc--;
	printf("wc: %zu\n", wc);
	printf("cc: %zu\n", cc);
}

int	main(int argc, char **argv)
{
	char	*input;

	argc = argc - 1 + 1;
	input = NULL;
	preparing_input(argv[1], argv[1][0]);
	return (0);
}
