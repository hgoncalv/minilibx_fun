/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hg_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:17:04 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:24:45 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_exit_code(t_cmd **curr, char **path, int *i)
{
	int	ret;

	ret = readline_check((*curr)->args);
	if (ret > 0)
	{
		_shell()->exit_code = ret;
		i[3] = 1;
		return (1);
	}
	if (check_if_builtin_not_pipe((*curr)))
	{
		_shell()->exit_code = run_builtin((*curr));
		i[3] = 1;
		return (1);
	}
	*path = ft_get_exec_path((*curr)->args);
	if (check_permission_denied(curr, path))
		return (1);
	if (check_command_not_found(curr, path))
		return (1);
	return (0);
}

int	run_if_first_level_builtins_set_path(t_cmd **curr, char **path,
		int *num_commands, int *i)
{
	if (*path)
		free(*path);
	*path = NULL;
	if (set_exit_code(curr, path, i))
	{
		*curr = (*curr)->next;
		*i = *i + 1;
		*num_commands = *num_commands - 1;
		i[3] = 0;
		return (1);
	}
	return (0);
}

void	execute_commands(pid_t pid, t_cmd *curr, char *path)
{
	if (pid == 0)
	{
		signal_default();
		if (check_if_builtin(curr))
			exit(run_builtin(curr));
		else
			exit(execve(path, curr->args, _shell()->envp));
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
}

void	pipe_commands_init(char **path, int **pipes, int *i, t_cmd *cmd)
{
	*path = NULL;
	*pipes = NULL;
	(i)[2] = count_cmds(cmd);
	i[0] = 0;
}

// i[0] = i
// i[1] = j
// i[2] = num_commands
// i[3] = just free index
void	pipe_commands(t_cmd *cmd)
{
	int		i[4];
	int		*pipes;
	pid_t	pid;
	t_cmd	*curr;
	char	*path;

	create_empty_files(cmd);
	pipe_commands_init(&path, &pipes, i, cmd);
	pipes = ft_realloc(pipes, sizeof(int) * 2 * i[2]);
	pipe_commands_build_pipes(pipes, i[2]);
	curr = cmd;
	while (curr)
	{
		i[1] = i[0] * 2;
		if (run_if_first_level_builtins_set_path(&curr, &path, &i[2], &i[0]))
			continue ;
		pid = fork();
		if (pid == 0)
			pipe_commands_dup_n_close_pipes(curr, pipes, i[2], i);
		execute_commands(pid, curr, path);
		i[0]++;
		curr = curr->next;
	}
	pipe_commands_cleanup(i[2], pipes, path);
}
