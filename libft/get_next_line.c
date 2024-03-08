/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caigner <caigner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:44:08 by codespace         #+#    #+#             */
/*   Updated: 2024/03/08 18:58:33 by caigner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	some_error(char *str)
{
	if (str)
		free(str);
	return (-1);
}

char	*putline(ssize_t index, char **left)
{
	char	*result;

	if (!*left)
		return (NULL);
	if (index <= 0)
	{
		if (index < 0 || **left == 0)
		{
			free(*left);
			*left = NULL;
			return (NULL);
		}
		result = *left;
		*left = NULL;
		return (result);
	}
	result = *left;
	*left = ft_substr(*left, index, (ft_strlen(*left) - index));
	result[index] = 0;
	return (result);
}

char	*get_ln(int fd, char **left, char *buffer)
{
	char	*new_left;
	char	*result;
	char	*tmp;
	ssize_t	bytes;

	tmp = ft_strchr(*left, '\n');
	while (tmp == NULL)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			return (putline(bytes, left));
		buffer[bytes] = 0;
		new_left = ft_strjoin(*left, buffer);
		free(*left);
		*left = new_left;
		tmp = ft_strchr(*left, '\n');
	}
	result = putline(tmp - *left + 1, left);
	return (result);
}

int	get_next_line(int fd, char **line, int flag)
{
	char		*buffer;
	static char	*left[1024];

	if (flag == 1)
	{
		free(left[fd]);
		left[fd] = NULL;
		return (1);
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (some_error(left[fd]));
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (some_error(left[fd]));
	if (!left[fd])
		left[fd] = ft_strdup("");
	*line = (get_ln(fd, &left[fd], buffer));
	free(buffer);
	return (1);
}

/* 
int main()
{
	int fd1;
	//int fd2;
    int i = 0;
	fd1 = open("text.txt", O_RDONLY);
	char *p;
		// while (1)
		// {
		// 	p = get_next_line(fd1);
		// 	if (p == 0)
		// 		return 0;
		// 	printf("return[%d] = %s\n ---------------------------\n", i, p);
		// 	free(p);
		// }
		p = get_next_line(fd1);
			printf("return[1] = %s\n -------------------------------\n", p);
			free(p);
		p = get_next_line(fd1);
			printf("return[2] = %s\n -------------------------------\n", p);
			free(p);
	close(fd1);
	// 	fd2 = open("test.txt", O_RDONLY);
	// printf("fd = %d", fd2);	
	// 	p = get_next_line(fd2);
	// 		printf("return[3] = %s\n -------------------------------\n", p);
	// 		free(p);
	// 	p = get_next_line(fd2);
	// 		printf("return[4] = %s\n -------------------------------\n", p);
	// 		free(p);
	// 	close(fd2);
	return 0;
} */