/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:56 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:43:22 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_matrix_remove_col_by_index(char **matrix, int index)
{
	int		col_num;
	char	**list;
	int		i;
	int		j;

	list = NULL;
	col_num = ft_matrix_get_num_col(matrix);
	list = ft_realloc(list, (col_num) * sizeof(char *));
	i = 0;
	j = 0;
	while (i < col_num - 1)
	{
		if (i == index)
			i++;
		list[j] = ft_strdup(matrix[i]);
		i++;
		j++;
	}
	list[j] = NULL;
	ft_matrix_free(matrix);
	return (list);
}

static int	hide_some(char **matrix, int *i)
{
	if ((matrix[*i][0] == '?' && matrix[*i][1] == '=') || ft_strnstr(matrix[*i],
			"./minishell", 11))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

static void	print_between_quotes(char **name_val, char **matrix, int i)
{
	if (name_val[1])
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr(name_val[1]);
		ft_putchar_fd('"', 1);
	}
	else
	{
		if (matrix[i][ft_strlen(matrix[i]) - 1] == '=')
			ft_putstr_fd("=\"\"", 1);
	}
}

void	ft_print_matrix_add_str2line_start(char **matrix, char *str, char *glue)
{
	int		i;
	char	**name_val;

	i = 0;
	while (matrix[i] != NULL)
	{
		if (hide_some(matrix, &i))
			continue ;
		ft_putstr_fd(str, 1);
		if (glue != NULL)
			ft_putstr_fd(glue, 1);
		name_val = ft_strsplit(matrix[i], '=');
		ft_putstr(name_val[0]);
		print_between_quotes(name_val, matrix, i);
		ft_putchar_fd('\n', 1);
		ft_matrix_free(name_val);
		i++;
	}
	return ;
}
