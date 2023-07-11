/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:36 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/01 22:00:30 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_files(t_file *file)
{
	t_file	*ptr;

	ptr = file->next;
	free(file->filename);
	if (file->heredoc)
	{
		remove(file->heredoc);
		free(file->heredoc);
	}
	free(file);
	file = NULL;
	if (ptr)
		free_files(ptr);
	return (1);
}

void	free_cmds(t_cmd *head)
{
	t_cmd	*next;

	if (!head)
		return ;
	while (head)
	{
		free_arrays(&(head->args));
		if (head->file && free_files(head->file))
			head->file = NULL;
		if (head->file_in && free_files(head->file_in))
			head->file_in = NULL;
		if (head->next)
			next = head->next;
		else
			break ;
		free(head);
		head = next;
	}
	if (head)
		free(head);
	_shell()->head = NULL;
}

void	minishell_clean(char **p2line)
{
	_shell()->valid_input = true;
	if (_shell()->valid_input)
	{
		if (_shell()->head)
			free_cmds(_shell()->head);
	}
	free(*p2line);
	*p2line = NULL;
	if (_shell()->p2matrix)
		free_arrays(_shell()->p2matrix);
	_shell()->p2matrix = NULL;
}
