/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:27 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 20:48:33 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_args(char ***p2matrix, int *i, t_cmd *cmd)
{
	char	*pipe_str;
	char	c;

	pipe_str = **p2matrix;
	c = *pipe_str;
	if (!is_redir(c) && !ft_strexact(pipe_str, "|"))
		cmd->args[(*i)++] = ft_strdup((pipe_str));
	else if (ft_strexact(pipe_str, "|"))
	{
		*p2matrix = *p2matrix + 1;
		return (0);
	}
	else
	{
		set_redir(cmd, &p2matrix);
		if (pipe_str && !ft_strexact(pipe_str, "|"))
			return (1);
	}
	if (pipe_str && !ft_strexact(pipe_str, "|"))
		*p2matrix = *p2matrix + 1;
	return (1);
}

void	expand_cmd_inplace(t_vars *v, char ***p2cmdargs, char ***args_expanded)
{
	(v->tmp_str) = ft_strdup((*p2cmdargs)[v->i]);
	remove_char(&(v->tmp_str), 2);
	(v->tmp_matrix) = ft_strsplit((v->tmp_str), ' ');
	v->l = ft_matrix_get_num_col(v->tmp_matrix);
	v->m += v->l;
	*args_expanded = ft_realloc(*args_expanded, v->m * sizeof(char *));
	v->k = 0;
	while ((v->tmp_matrix)[v->k])
		(*args_expanded)[v->j++] = ft_strdup((v->tmp_matrix)[v->k++]);
	free((v->tmp_str));
	free_arrays(&(v->tmp_matrix));
}

void	expand_cmd_args(char ***p2cmdargs)
{
	char	**args_expanded;
	t_vars	v;

	ft_inicialize_vars(&v);
	v.m = ft_matrix_get_num_col(*p2cmdargs) + 1;
	args_expanded = ft_calloc(v.m, sizeof(char **));
	while ((*p2cmdargs)[v.i])
	{
		if ((*p2cmdargs)[v.i][0] == 2)
			expand_cmd_inplace(&v, p2cmdargs, &args_expanded);
		else if ((*p2cmdargs)[v.i] != NULL)
			args_expanded[v.j++] = ft_strdup((*p2cmdargs)[v.i]);
		v.i++;
	}
	args_expanded[v.j] = NULL;
	free_arrays(p2cmdargs);
	*p2cmdargs = args_expanded;
}

t_cmd	*init_tcmd(char ***p2matrix, int i)
{
	t_cmd	*cmd;
	char	**matrix;

	matrix = *p2matrix;
	cmd = ft_inicialize_cmd(matrix);
	while (**p2matrix != NULL)
	{
		if (!process_args(p2matrix, &i, cmd))
			break ;
	}
	cmd->args[i] = NULL;
	expand_cmd_args(&(cmd->args));
	return (cmd);
}

bool	add_cmds(char **matrix)
{
	validate_matrix(matrix);
	if (_shell()->valid_input == false)
	{
		printf("SYNTAX ERROR: unexpected token ...\n");
		return (false);
	}
	else
		build_cmds_delete_matrix(matrix);
	init_heredoc();
	return (true);
}
