/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:50 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:32:03 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_handler(int signum)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, 0);
	if (signum == SIGINT)
	{
		if (pid == -1)
		{
			_shell()->exit_code = 130;
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			write(1, "\n", 1);
	}
	if (signum == SIGQUIT)
	{
		if (pid == -1)
			_shell()->exit_code = 131;
		write(1, "\n", 1);
	}
}

void	signal4main(void)
{
	signal(SIGINT, ft_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
