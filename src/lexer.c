/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:16:46 by miheider          #+#    #+#             */
/*   Updated: 2024/02/05 19:15:07 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

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

void	preparing_input(char *input)
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
			printf("0\n");
		}
		while (*input && *input != ' ')
		{
			 if (*input && *input == 46) // . (dot) instead of " and '
            {
				if (*(input - 1) == 32)
				{
                	input++;
                	cc++;
				}
				else if (*(input - 1) != 32)
				{
					input++;
					cc++;
					wc++;
				}
                while (*input && *input != 46)
                {
                    cc++;
                    input++;
                    printf("%zu\n", cc);
                }

                if (*input && *input == 46 && *(input + 1) == 32)
				{
					cc++;
					input++;
				}
				else if (*input && *input == 46 && *(input + 1) != 32)
                {
                    cc++;
					input++;
					wc++;
                }
            }
			else if (check_string(*input, *(input + 1)) == 0)
			{
				cc++;
				input++;
				printf("Hallo\n");
			}
			else if (check_string(*input, *(input + 1)) == 3)
			{
				printf("%c\n", *input);
				input++;
				cc++;
				while (*input != 34 || *input != 39)
				{
					input++;
					cc++;
				}
				input++;
				cc++;
				wc++;
			}
			else if (check_string(*input, *(input + 1)) == 1)
			{
				if (*(input - 1) == 32 && (*(input + 1) != 32 || *(input + 1) == '\0'))
				{
					wc++;
					cc++;
					input++;
					printf("1\n");
				}
				else if (*(input - 1) != 32 && ((*(input + 1) == 32) || *(input + 1) == '\0'))
				{
					wc++;
					cc++;
					input++;
					printf("2\n");
				}
				else if (*(input - 1) == 32 && *(input + 1) == 32)
				{
					cc++;
					input++;
					printf("3\n");
				}
				else
				{
					wc += 2;
					cc++;
					input++;
					printf("4\n");
				}
				printf("Wie\n");
			}
			else if (check_string(*input, *(input + 1)) == 2)
			{
				if (*(input - 1) == 32 && (*(input + 2) != 32 || *(input + 2) == '\0'))
				{
					wc += 1;
					cc += 2;
					input += 2;
				}
				else if (*(input -1) != 32 && (*(input + 2) == 32 || *(input + 2) == '\0'))
				{
					wc += 1;
					cc += 2;
					input += 2;
				}
				else if (*(input - 1) == 32 && *(input + 2) == 32)
				{
					cc += 2;
					input += 2;
				}
				else
				{
					wc += 2;
					cc += 2;
					input += 2;
				}
				printf("gehts?\n");
			}
			else if (check_string(*input, *(input + 1)) == 4)
			{
				if (/**input &&*/ *input != ' ')
				{
				//	wc++;
					cc++;
					input++;
					printf("dir\n");
				}
				else if (/**input &&*/ *(input - 1) == ' ')
				{
					wc++;
					cc++;
					input++;
					printf("denn?");
				}
				printf("heute\n");
			}
		}
	}
	printf("wc: %zu\n", wc);
	printf("cc: %zu\n", cc);
}



int	main(int argc, char **argv)
{
	char	*input;

	argc = argc - 1 + 1;
	input = NULL;
	preparing_input(argv[1]);
	return (0);
}