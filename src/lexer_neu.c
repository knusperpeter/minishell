
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

char    *ft_strchr (const char *s, int c)               //del - libft
{
    int a;

    a = 0;
    while (s[a])
    {
        if (s[a] == (char)c) 
            return ((char *)&s[a]);
        a++;
    }
    if ((char)c == '\0')
        return ((char *)&s[a]);
    return (NULL);
}

char *ft_strtok(char *s1, const char *delim)
{
    static char *str;
    char        *start;
    int         in_quotes;

    in_quotes = 0;
    if (s1)
        str = s1;
    if (!str)
        return NULL;
    while (*str && strchr(delim, *str))
        str++;
    if (*str == '\0')
        return NULL;
    if (*str == '.')
    {
        in_quotes = 1;
        str++;
    }
    start = str;
    if (in_quotes)
    {
        while (*str && *str != '.')
            str++;
        if (*str == '.')
            str++;
    }
    else
    {
        while (*str && !strchr(delim, *str))
            str++;
    }
    if (*str == '\0')
        str = NULL;
    else
        *str++ = '\0';
    return start;
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)   //del - libft
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

int check_char(char *character)                     //analysing character                     //analysing character
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


void tokenize_input(char *input)
{
    char **result = NULL;
    char *token;
    int index = 0;

    token = ft_strtok(input, " ");
    while (token != NULL) {
        result = realloc(result, (index + 1) * sizeof(char *));
        if (result == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        result[index++] = strdup(token);
        token = ft_strtok(NULL, " ");
    }

    for (int i = 0; i < index; i++) {
        printf("result[%d]: ___%s___\n", i, result[i]);
        free(result[i]); // Free each token
    }
    free(result); // Free array of tokens
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
    tokenize_input(&new_string[0]);
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
                    wc++;
                cc++;
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
    printf("wc: %d\n", wc);                     // del wc
    printf("cc: %d\n", cc);                     // del cc
    set_up_array(wc, cc, string);
}

int  main(int argc, char **argv)
{
    argc = argc - 1 + 1;
    prep_input(argv[1]);
    return (0);
}
