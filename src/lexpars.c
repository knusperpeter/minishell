
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

int check_char(char *character)                     //analysing character
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
/*
int     check_parsing(char *first)
{
    int i = 0;

    if (first[0] == '<' && first[1] == '\0')
        return (1);
    else if (first[0] == '<' && first[1] == '<')
        return (2);
    else if (first[0] == '>' && first[1] == '\0')
        return (3);
    else if (first[0] == '>' && first[1] == '>')
        return (4);
    else if (first[0] == '|' && first[1] == '\0')
        return (5);
    while (first[i])
    {
        if (first[i] == '\' && first[i + 1] == '.')
            return (6);
        i++;
    }
    return (0);
}

void    set_up_list(char *token, int value)
{
    t_env   data;



    
}




void    parse_tokens(char **tokens, int index)
{
    int i = 0;
    int ret = 0;
    int len = 0;

    while (tokens[i])
    {
        ret = check_parsing(tokens[i]);
        len = ft_strlen(tokens);
        if (ret == 1 && len == 1)                        // <
            set_up_list(tokens[i], REDIR_L);
        else if (ret == 2 && len == 2)                   // <<
            set_up_list(tokens[i], HEREDOC_L);
        else if (ret == 3 && len == 1)                   // >
            set_up_list(tokens[i], REDIR_R);
        else if (ret == 4 && len == 2)                   // >>
            set_up_list(tokens[i], APPEND);
        else if (ret == 5 && len == 1)                   // |
            set_up_list(tokens[i], PIPE);
        else if (ret == 6)                               // "
            set_up_list(tokens[i], STRING);
        else 
            set_up_list(tokens[i], VOID);
        i++;
    }
}*/
/*erhält nach pipe tokenizeden tokens und tokenized sie nach spaces und speicher sie in linked list*/
/*
void    tokenize_tokens(char **tokens, int index)
{
    int i = 0;
    char *tok;
    
    while (i < index)
    {
        tok = ft_strtok(tokens[i], " ");
        while (tok != NULL)
        {

        }
    }

}*/

/*erstellt array mit den nach pipes getokenizeden tokens*/
void tokenize_input(char *input)
{
    char **result = malloc(num_tokens * sizeof(char *));
    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    char *token = ft_strtok(input, "|");
    int index = 0;
    while (token != NULL && index < num_tokens)
    {
        result[index++] = strdup(token);
        token = ft_strtok(NULL, "|");
    }

    if (index < num_tokens)
    {
        fprintf(stderr, "Expected %d tokens, but found only %d tokens.\n", num_tokens, index);
        // You may handle this situation according to your needs.
    }

   // tokenize_tokens(result, index);

    for (int i = 0; i < index; i++)
    {
        printf("result[%d]: ___%s___\n", i, result[i]);
        free(result[i]); // Free each token
    }
    free(result); // Free array of tokens
}
/*richtet input zum tokenizen her (genau 1 space)*/
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
            if (check_char(&input[i]) == 2)
            {
                if (input[i - 1] != ' ' && i > 0)
                {
                    new_string[j] = ' ';
                    j++;
                }
                new_string[j] = input[i];
                i++;
                j++;
                while (check_char(&input[i]) != 2)
                {
                    new_string[j] = input[i];
                    i++;
                    j++;
                }
                if (check_char(&input[i]) == 2)
                {
                    new_string[j] = input[i];
                    j++;
                    i++;
                    if (input[i] == ' ')
                        break;
                    if (input[i] != ' ' && input[i] != '\0')
                    {
                        new_string[j] = ' ';
                        j++;
                    }
                }
            }
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
    tokenize_input(&new_string[0]);
    printf("erg:___%s___\n", new_string);
    free(new_string);
}
/*zählt Wörter und Characters im input*/
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
