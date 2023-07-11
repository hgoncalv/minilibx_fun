/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:37 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:24:12 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	read_and_write_input(char *word, t_file *file)
{
	char	*line;
	char	*filename;

	line = NULL;
	while (1)
	{
		filename = ft_strdup(file->filename);
		ft_stradd(&filename, ">>");
		line = readline(filename);
		if (line == NULL || ft_strexact(line, word))
		{
			ft_putchar('\n');
			break ;
		}
		write(file->fd, line, ft_strlen(line));
		write(file->fd, "\n", 1);
		free(line);
		line = NULL;
		free(filename);
	}
	if (line)
		free(line);
	free(filename);
	close(file->fd);
}

static void	ask_for_input(char *word, t_file *file)
{
	char	*tmp;
	pid_t	pid;
	int		status;

	tmp = ft_strdup(".tmp.");
	file->heredoc = ft_strjoin(tmp, file->filename);
	file->fd = open(file->heredoc, O_RDWR | O_CREAT | O_TRUNC, 0777);
	free(tmp);
	pid = fork();
	if (pid == 0)
	{
		signal_default();
		read_and_write_input(word, file);
		_exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	signal4main();
}

void	init_heredoc(void)
{
	t_cmd	*cmd;
	t_file	*ptr;

	cmd = _shell()->head;
	while (cmd != NULL)
	{
		ptr = cmd->file_in;
		while (ptr)
		{
			if (ptr && (ptr->type == HEREDOC))
				ask_for_input(ptr->filename, ptr);
			ptr = ptr->next;
		}
		cmd = cmd->next;
	}
}
