/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:05 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/07 20:20:54 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_envp	*_shell(void)
{
	static t_envp	enviroment;

	return (&enviroment);
}

void	init_shell(char **ev)
{
	t_envp	*shell;

	shell = _shell();
	shell->envp = ft_matrix_dup(ev, 0);
	shell->exit_code = 0;
	shell->exit = 0;
	shell->leave_heredoc = 0;
	shell->p2matrix = NULL;
	shell->p2line = NULL;
	_shell()->valid_input = true;
}

void	close_shell(void)
{
	if (_shell()->head)
		free_cmds(_shell()->head);
	if (_shell()->envp)
		free_arrays(&(_shell()->envp));
}
