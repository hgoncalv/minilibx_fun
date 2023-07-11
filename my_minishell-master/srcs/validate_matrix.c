/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_matrix.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 01:46:20 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/03 22:51:17 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	validate_matrix(char **matrix)
{
	char	c;
	int		i;
	int		j;

	i = 0;
	while (matrix[i])
	{
		j = 0;
		while (matrix[i][j])
		{
			c = matrix[i][j];
			validate_quotes(matrix, &i, &j, c);
			validate_pipe(matrix, &i, &j);
			validate_redir(matrix, &i, &j);
			if (matrix[i][j] != '\0')
				j++;
		}
		i++;
	}
}

void	validate_quotes(char **matrix, int *i, int *j, char c)
{
	if (matrix[*i][*j] == '\"' || matrix[*i][*j] == '\'')
	{
		c = matrix[*i][*j];
		(*j)++;
		while (matrix[*i][*j] && matrix[*i][*j] != c)
			(*j)++;
		if (matrix[*i][*j] != '\0')
			(*j)++;
	}
}

void	validate_pipe(char **matrix, int *i, int *j)
{
	if (matrix[*i][*j] == '|')
	{
		if (*i == 0 && *j == 0)
			_shell()->valid_input = false;
		else if (matrix[*i + 1] && matrix[*i + 1][0] == '|')
			_shell()->valid_input = false;
		else if (!matrix[*i + 1])
			_shell()->valid_input = false;
		else if (matrix[*i + 1] && matrix[*i + 2] && matrix[*i + 2][0] == '|'
				&& matrix[*i + 1][0] == ' ' && matrix[*i][0] == '|')
			_shell()->valid_input = false;
		else if (matrix[*i] && matrix[*i][1] && matrix[*i][1] == '|')
			_shell()->valid_input = false;
	}
}

void	validate_redir(char **matrix, int *i, int *j)
{
	if (matrix[*i][*j] == '>')
		ft_validate_redir_output_matrix(matrix, i);
	if (matrix[*i][*j] == '<')
		ft_validate_redir_input_matrix(matrix, i);
}
