/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hg_loop_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:17:04 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/07 02:12:17 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pipe_commands_cleanup(int num_commands, int *pipes, char *path)
{
	int	i;
	int	status;
	int	wpid;

	i = 0;
	while (i < 2 * num_commands)
	{
		close(pipes[i]);
		i++;
	}
	i = 0;
	while (i < num_commands)
	{
		wpid = waitpid(-1, &status, WUNTRACED);
		pipe_get_return_from_child(status);
		(void)wpid;
		i++;
	}
	if (_shell()->exit_code == 131)
		printf("Quit (core dumped)\n");
	if (path)
		free(path);
	free(pipes);
}

void	pipe_commands_build_pipes(int *pipes, int num_commands)
{
	int	i;

	if (pipes == NULL)
	{
		perror("malloc");
		exit(1);
	}
	i = 0;
	while (i < num_commands)
	{
		if (pipe(pipes + i * 2) < 0)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	pipe_commands_dup_n_close_pipes(t_cmd *curr, int *pipes,
		int num_commands, int *i)
{
	redirect_input(curr);
	if (i[0] > 0)
	{
		if (dup2(pipes[i[1] - 2], STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
	}
	if (i[0] < num_commands - 1)
	{
		if (dup2(pipes[i[1] + 1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
	}
	i[2] = 0;
	while (i[2] < 2 * num_commands)
	{
		close(pipes[i[2]]);
		i[2]++;
	}
	redirect_output(curr);
}

int	check_permission_denied(t_cmd **curr, char **path)
{
	if (*path && access(*path, X_OK) != 0)
	{
		printf("bash: %s: Permission denied\n", ((*curr)->args)[0]);
		_shell()->exit_code = 126;
		return (1);
	}
	return (0);
}

int	check_command_not_found(t_cmd **curr, char **path)
{
	if (!check_if_builtin((*curr)) && !*path)
	{
		if ((*curr)->args[0] && ((*curr)->args[0][0] == '.'
			|| (*curr)->args[0][0] == '/'))
			printf("minishell: %s: No such file or directory\n",
				((*curr)->args)[0]);
		else
			printf("minishell: %s: command not found\n", ((*curr)->args)[0]);
		_shell()->exit_code = 127;
		return (1);
	}
	return (0);
}
