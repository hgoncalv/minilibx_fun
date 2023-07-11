/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:07 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/07 01:50:33 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_new_redir(int c)
{
	return (c == '<' || c == '>');
}

void	print_tcmd(t_cmd *cmd)
{
	int		i;
	t_file	*file;

	if (!cmd)
		return ;
	file = NULL;
	i = -1;
	if (cmd->args[0] == NULL)
		printf("NO ARGS\n");
	while (cmd->args[++i])
		printf("ARGS[%d]= %s\n", i, cmd->args[i]);
	if (cmd->file)
	{
		file = cmd->file;
		printf("FILE_OUT: %s (%d)\n", file->filename, file->type);
	}
	if (cmd->file_in)
	{
		file = cmd->file_in;
		printf("FILE_IN: %s (%d)\n", file->filename, file->type);
	}
	printf("__________\n");
	if (cmd->next)
		print_tcmd(cmd->next);
}

void	free_arrays(char ***p2matrix)
{
	int	i;

	i = 0;
	while ((*p2matrix)[i])
	{
		free((*p2matrix)[i]);
		(*p2matrix)[i++] = NULL;
	}
	free(*p2matrix);
	*p2matrix = NULL;
}

void	print_arrays(char **a)
{
	int	i;

	i = -1;
	while (a[++i])
		printf("[%d] %s\n", i, a[i]);
}