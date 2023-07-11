/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hg_loop_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:17:04 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:30:51 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pipe_get_return_from_child(int status)
{
	if (WTERMSIG(status) == SIGINT)
		_shell()->exit_code = 130;
	else if (WTERMSIG(status) == SIGQUIT)
		_shell()->exit_code = 131;
	else
		_shell()->exit_code = status / 256;
}

void	create_empty_files(t_cmd *cmd)
{
	t_file	*file;
	int		output;

	if (!cmd)
		return ;
	file = NULL;
	if (cmd->file)
	{
		file = cmd->file;
		if (file->type == R_OUT)
		{
			output = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(output);
		}
		if (cmd->next)
			create_empty_files(cmd->next);
	}
}
