/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:11:32 by miheider          #+#    #+#             */
/*   Updated: 2024/03/11 21:40:11 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	prep_input(char *input)
{
	int	i;
	int	cc;

	i = 0;
	cc = 0;
	while (input[i])
	{
		if (input[i] == '.')
		{
			i++;
			cc++;
			while (input[i] != '.')
			{
				cc++;
				i++;
			}
		}
		if (input[i] != ' ' || input[i] == '<')	//ersetzen durch chech_char == 1
			cc++;
		while (input[i] && input[i] != ' ')
		{
			cc++;
			i++;
		}
		while (input[i] && input[i] == ' ')
			i++;
	}
	printf("%d\n", cc);
}

int	main(void)
{
	char *a = "hallo | .hallo du | dddd. geht";
	prep_input(a);
	return (0);
}