/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:14 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/02 01:57:05 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_validate_pipe_matrix(char **line, int *i)
{
	if (line[*i])
	{
		if (line[*i][1])
		{
			if (line[*i][1] == '|')
				_shell()->valid_input = false;
		}
	}
}

void	ft_validate_redir_output_matrix(char **line, int *i)
{
	if (line[*i + 1])
	{
		if ((line[*i + 1][0] == '>') || (line[*i + 1][0] == '<'))
			_shell()->valid_input = false;
	}
	else
		_shell()->valid_input = false;
}

void	ft_validate_redir_input_matrix(char **line, int *i)
{
	if (line[*i + 1])
	{
		if ((line[*i + 1][0] == '>') || (line[*i + 1][0] == '<'))
			_shell()->valid_input = false;
	}
	else
		_shell()->valid_input = false;
}
