/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_build2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:28 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 14:44:17 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	set_redir_back(t_cmd *cmd, char ****str)
{
	if (ft_strexact(***str, "<"))
	{
		cmd->type = R_IN;
		**(str) = **str + 1;
		create_infile(***str, cmd);
	}
	else if (ft_strexact(***str, "<<"))
	{
		cmd->type = HEREDOC;
		**(str) = **str + 1;
		create_infile(***str, cmd);
	}
}

void	set_redir(t_cmd *cmd, char ****str)
{
	if (ft_strexact(***str, ">"))
	{
		cmd->type = R_OUT;
		**(str) = **str + 1;
		create_file(***str, cmd);
	}
	else if (ft_strexact(***str, ">>"))
	{
		cmd->type = R_APP;
		**(str) = **str + 1;
		create_file(***str, cmd);
	}
	set_redir_back(cmd, str);
	**(str) = **str + 1;
	if (ft_strexact(***str, "|"))
		return ;
	if (***str && is_redir(****str))
		set_redir(cmd, str);
}

void	build_cmds_delete_matrix(char **matrix)
{
	t_cmd	*p2cmd;
	t_cmd	*p2next_cmd;

	p2cmd = init_tcmd(&matrix, 0);
	_shell()->head = p2cmd;
	while (*matrix != NULL)
	{
		p2next_cmd = init_tcmd(&matrix, 0);
		p2cmd->next = p2next_cmd;
		p2next_cmd->prev = p2cmd;
		p2cmd = p2cmd->next;
	}
	if (_shell()->p2matrix)
	{
		free_arrays(_shell()->p2matrix);
		_shell()->p2matrix = NULL;
	}
}
