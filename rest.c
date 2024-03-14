/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:11:32 by miheider          #+#    #+#             */
/*   Updated: 2024/03/13 14:37:23miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

int check_char(char *character)
{
	char    *special;
	int     i;

	i = 0;
	special = "*?[]()<>#";
	if (*character == '\'' || *character == '\"' || *character == '.')
		return (2);
	while (i < 10)
	{
		if (special[i] == *character)
			return (1);
		i++;
	}
	return (0);
}

char *set_up_array(int cc, char *input)
{
	char *new_string;
	int i = 0;
	int j = 0;

	new_string = (char *)malloc(sizeof(char) * (cc + 1));
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
				new_string[j++] = input[i++];
				while (input[i] && check_char(&input[i]) != 2 )
					new_string[j++] = input[i++];
				if (input[i] && check_char(&input[i]) == 2 )
				{
					new_string[j++] = input[i++];
					if (input[i] == ' ')
						break;
					if (input[i] != ' ' && input[i] != '\0')
						new_string[j++] = ' ';
				}
			}
			if ((i > 0 && check_char(&input[i]) == 0 &&
				check_char(&input[i - 1]) == 1) ||
				(check_char(&input[i]) == 1 &&
				check_char(&input[i - 1]) == 0 &&
				input[i - 1] != ' ') || (check_char(&input[i]) == 1
				&& check_char(&input[i - 1]) == 1 &&
				input[i] != input[i - 1]))
				new_string[j++] = ' ';
			new_string[j++] = input[i++];
		}
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i - 1] == ' ' && input[i] != '\0')
			new_string[j++] = ' ';
	}
	new_string[j] = '\0';
	printf("___%s___\n", new_string);
	return (new_string);
}

void	count_up(int *i, int *cc)
{
		(*i)++;
		(*cc)++;
}


int	prep_input(char *input)
{
	int	i;
	int	cc;

	i = 0;
	cc = 0;
	while (input[i] == ' ')
		i++;
	while (input[i])
	{
		if (i > 0 && cc != 0 && input[i] == '<' && input[i - 1] != ' ' &&
			input[i - 1] != '<' && input[i - 1] != '.')
			cc++;
		if (input[i] == '<' && input[i + 1] != ' ' && input[i + 1] != '<' &&
			input[i + 1] != '\0' && input[i] != '.')//ersetzen mit check_char
			cc++;
		if (i > 0 && input[i] && input[i] == '.')	//ersetzen mit ' und "
		{
			if (input[i - 1] != ' ')
				cc--;
			count_up(&i, &cc);
			if (i > 1 && input[i - 2] != ' ')
				cc++;
			while (input[i] != '.')
				count_up(&i, &cc);
			if (input[i] == '.' && input[i + 1] != ' ' && input[i + 1] != '\0')
				cc++;
			else if(input[i + 1] != '\0' || input[i + 1] == ' ')
				count_up(&i, &cc);
			else if (input[i + 1] != '\0')
				break;
		}
		if (i > 0 && cc != 0 && input[i] && input[i] != ' ' &&
			input[i - 1] == ' ' && input[i] != '.')
			cc++;
		if (i > 0 && cc != 0 && input[i] && input[i] == ' ' &&
			input[i + 1] == '.')
			cc++;
		if (input[i] && input[i] != ' ')
			cc++;
		if (input[i] == '<' && input[i + 1] == '.')
			cc--;
		i++;
	}
	printf("cc: %d\n", cc);
	set_up_array(cc, input);
	return (cc);
}

int	main(void)
{
	char *a = "    test.hallo. <<me ";

	prep_input(a);
	return (0);
}