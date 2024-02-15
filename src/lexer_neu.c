
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

int check_char(char *character)
{
    char special[11] = "*?[]()<>|#\"";
    int i = 0;

    if (*character == '.' || *character == '\'' || *character == '\"')
        return (2);

    while (i < 11)
    {
        if (special[i] == *character)
            return (1);
        i++;
    }
    return (0);
}

void set_up_array(int wc, int cc, char *input)
{
    char *new_string;
    int i = 0;
    int j = 0;

    new_string = (char *)malloc(sizeof(char) * (wc + cc));
    if (!new_string)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    while (input[i] == ' ')
        i++;
    while (input[i])
    {
        while (input[i] && input[i] != ' ')
        {
            if (check_char(&input[i]) == 0 && check_char(&input[i - 1]) == 1)
            {
                new_string[j] = ' ';
                j++;
            }
            if (check_char(&input[i]) == 1 && check_char(&input[i - 1]) == 0 && input[i - 1] != ' ' && i > 0)
            {
                new_string[j] = ' ';
                j++;
            }
            if (check_char(&input[i]) == 1 && check_char(&input[i - 1]) == 1 && input[i] != input[i - 1])
            {
                new_string[j] = ' ';
                j++;
            }
            new_string[j] = input[i];
            i++;
            j++;        
        }
        while (input[i] == ' ')
            i++;
        if (input[i - 1] == ' ' && input[i] != '\0')
        {
            new_string[j] = ' ';
            j++;
        }
    }
    new_string[j] = '\0';
    printf("erg:___%s___\n", new_string);
    free(new_string);
}

void prep_input(char *input)
{
    int wc = 0;
    int cc = 0;
    char *string;

    string = input;
    if (check_char(input) == 1)
        wc--;
    while (*input)
    {
        while (*input == ' ')
            input++;
        if (*input != ' ')
        {
            if (*input == '\0')
                break;
            wc++;
        }
        while (*input && *input != ' ')
        {
            if (check_char(input) == 1 && ((*(input - 1) == ' ') || (*(input - 1) == *input)))
                cc++;
            else if (check_char(input) == 1 && *(input - 1) != ' ')
            {
                wc++;
                cc++;
            }
            else if (check_char(input) == 0 && (check_char(input - 1) == 1 || check_char(input - 1) == 2))
            {
                wc++;
                cc++;
            }
            else if (check_char(input) == 0 && (check_char(input - 1) != 1)) 
                cc++;
            else
            {
                if (*(input - 1) != ' ')
                {
                    wc++;
                    cc++;
                }
                else 
                {
                    cc++;
                }
                input++;
                while (check_char(input) != 2 && *input)
                {
                    cc++;
                    input++;
                }
                cc++;
            }
            input++;
        }
    }
    printf("wc: %d\n", wc);
    printf("cc: %d\n", cc);
    set_up_array(wc, cc, string);
}

int  main(int argc, char **argv)
{
    argc = argc - 1 + 1;
    prep_input(argv[1]);
    return (0);
}
