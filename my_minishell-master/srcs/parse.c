/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:00 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:53:06 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
//parsing of quotes

static char	*buffer_quotes(char **p2line, char c, char *str, int i)
{
	str[(i)++] = *(*p2line)++;
	while (**p2line && **p2line != c)
		str[(i)++] = *(*p2line)++;
	if (**p2line == c)
		str[(i)++] = *(*p2line)++;
	return (str);
}

char	*buffer_scan_for_quotes(char *str)
{
	int	to_break_now;

	to_break_now = 0;
	while (check_if_str_need_quotes(str))
	{
		to_break_now = ft_do_quote(&str);
		if (to_break_now)
			break ;
	}
	return (str);
}

static void	line_to_row_quotes(char **p2line, char **p2row, int *i)
{
	if (ft_strexist("'\"\2", **p2line) && *(*p2line) != '\0' && *(*p2line
			+ 1) == **p2line)
		(*p2line) += 2;
	if (ft_strexist("'\"\2", **p2line) && *(*p2line) != '\0' && (**p2line
			+ 1 != **p2line))
		(*p2row) = buffer_quotes(&(*p2line), **p2line, (*p2row), *i);
	else if (**p2line)
		(*p2row)[(*i)++] = *(*p2line)++;
}

//its a collumn but i find it visualy more intuitive to treat as a row
//think of a traversed matrix ;)
// return (buffer_scan_for_quotes(row));
static char	*line_to_row(char **p2line)
{
	char	*row;
	int		i;

	row = ft_calloc(sizeof(char), (ft_strlen(*p2line) + 1));
	i = 0;
	while (**p2line)
	{
		i = ft_strlen(row);
		if (ft_isspace(**p2line))
			break ;
		if (is_redir(**p2line))
		{
			if (i != 0)
				return (row);
			row[i++] = *(*p2line)++;
			if (**p2line == row[i - 1])
				row[i++] = *(*p2line)++;
			return (row);
		}
		line_to_row_quotes(p2line, &row, &i);
	}
	check_rowquotes_free_if_needed(&row);
	return (row);
}

char	**line_to_matrix(char **p2line)
{
	char	**matrix;
	int		i;
	char	*line;

	line = *p2line;
	matrix = NULL;
	matrix = ft_realloc(matrix, sizeof(char *) * (r_size(line) + 1));
	i = 0;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (!line || ft_strlen(line) == 0)
			break ;
		matrix[i++] = line_to_row(&line);
		if (!matrix[i - 1])
		{
			ft_matrix_free(matrix);
			return (NULL);
		}
	}
	matrix[i] = 0;
	free(*p2line);
	*p2line = NULL;
	return (matrix);
}
