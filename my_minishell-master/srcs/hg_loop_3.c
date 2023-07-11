/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hg_loop_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:17:04 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/02 01:01:17 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_cmds(t_cmd *cmd)
{
	int		i;
	t_cmd	*curr;

	i = 0;
	curr = cmd;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

int	open_output_file(t_file *file)
{
	int	outputfd;

	if (file->type == R_OUT)
		outputfd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		outputfd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outputfd < 0)
	{
		perror("open");
		exit(1);
	}
	return (outputfd);
}

void	redirect_output(t_cmd *cmd)
{
	int		outputfd;
	t_file	*file;

	if (!(cmd->file && (cmd->file->type == R_OUT || cmd->file->type == R_APP)))
	{
		return ;
	}
	file = cmd->file;
	outputfd = open_output_file(file);
	if (dup2(outputfd, 1) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(outputfd);
}

int	open_input_file(t_file *ptr)
{
	int	fd;

	if (ptr->type == HEREDOC)
		fd = open(ptr->heredoc, O_RDONLY, 0777);
	else
	{
		if (access(ptr->filename, F_OK) == -1)
		{
			perror(ptr->filename);
			exit(1);
		}
		fd = open(ptr->filename, O_RDONLY, 0777);
	}
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	return (fd);
}

void	redirect_input(t_cmd *cmd)
{
	t_file	*ptr;

	ptr = cmd->file_in;
	if (!ptr)
	{
		return ;
	}
	ptr->fd = open_input_file(ptr);
	cmd->fd_in = ptr->fd;
	if (dup2(ptr->fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(ptr->fd);
}
