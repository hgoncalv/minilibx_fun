/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:02 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 21:02:03 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_quote(char **output)
{
	char	*buff;
	char	*tmp;

	buff = readline("> ");
	if (buff == NULL)
		return (2);
	add_history(buff);
	tmp = ft_strdup(*output);
	free(*output);
	*output = ft_concat3(tmp, "\n", buff);
	free(buff);
	free(tmp);
	return (0);
}

void	remove_starting_spaces(char **line)
{
	char	*str;
	int		i;

	if (!line || !*line)
		return ;
	i = 0;
	str = *line;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (i > 0 && str[i])
	{
		str = ft_strdup(*line + i);
		free(*line);
		*line = str;
	}
}

void	remove_ending_spaces(char **line)
{
	char	*str;
	int		i;

	if (!line || !*line)
		return ;
	i = 0;
	str = *line;
	while (str[i])
		i++;
	while (i > 0)
	{
		if (ft_isspace(str[i]))
			i--;
		else
			break ;
	}
	if (str[i])
		str[i] = '\0';
}

void	remove_start_end_spaces(char **line)
{
	remove_starting_spaces(line);
	remove_ending_spaces(line);
}

int	readline_check(char **p2line)
{
	char	*line;

	line = *p2line;
	if (ft_strexact("''", line) || ft_strexact("\"\"", line)
		|| ft_strexact("..", line) || ft_strexact(".", line))
	{
		if (ft_strexact(".", line))
		{
			printf("bash: .: filename argument required\n");
			printf("\t.: usage: . filename [arguments]\n");
		}
		else if (ft_strexact("..", line))
			printf("bash: ..: command not found\n");
		else
			printf("bash: : command not found\n");
		return (1);
	}
	return (0);
}
