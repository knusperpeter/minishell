/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rest.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:11:32 by miheider          #+#    #+#             */
/*   Updated: 2024/03/12 23:47:32 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include <stdio.h>

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
		if (i > 0 && cc != 0 && input[i] == '<' && input[i - 1] != ' ' && input[i - 1] != '<' && input[i - 1] != '.')
			cc++;
		if (input[i] == '<' && input[i + 1] != ' ' && input[i + 1] != '<' && input[i + 1] != '\0' && input[i] != '.')	//ersetzen mit check_char
			cc++;
		if (i > 0 && input[i] && input[i] == '.')	//ersetzen mit ' und "
		{
			if (input[i - 1] != ' ')
				cc--;
			cc++;
			i++;
			if (i > 1 && input[i - 2] != ' ')
				cc++;
			while (input[i] != '.')
			{
				cc++;
				i++;
			}
			if (input[i] == '.' && input[i + 1] != ' ' && input[i + 1] != '\0')
				cc++;
			else if(input[i + 1] != '\0' || input[i + 1] == ' ')
			{
				cc++;
				i++;
			}
			else if (input[i + 1] != '\0')
				break;
		}
		if (i > 0 && cc != 0 && input[i] && input[i] != ' ' && input[i - 1] == ' ' && input[i] != '.')
			cc++;
		if (i > 0 && cc != 0 && input[i] && input[i] == ' ' && input[i + 1] == '.')
			cc++;
		if (input[i] && input[i] != ' ')
			cc++;
		if (input[i] == '<' && input[i + 1] == '.')
			cc--;
		i++;
	}
	printf("cc: %d\n", cc);
	return (cc);
}

int	main(void)
{
	char *a = "test.hallo < ca. <<";

	prep_input(a);
	return (0);
}