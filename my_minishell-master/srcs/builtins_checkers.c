/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:24 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/06 17:04:38 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_if_builtin(t_cmd *cmd)
{
	if (ft_strchr(cmd->args[0], '='))
		return (1);
	if (ft_strexact("cd", cmd->args[0]))
		return (1);
	if (ft_strexact("export", cmd->args[0]))
		return (1);
	if (ft_strexact("unset", cmd->args[0]))
		return (1);
	if (ft_strexact("exit", cmd->args[0]))
		return (1);
	if (ft_strexact("pwd", cmd->args[0]))
		return (1);
	if (ft_strexact("echo", cmd->args[0]))
		return (1);
	if (ft_strexact("env", cmd->args[0]))
		return (1);
	return (0);
}

int	check_if_builtin_not_pipe(t_cmd *cmd)
{
	if (!cmd->next && !cmd->prev)
	{
		if (ft_strchr(cmd->args[0], '='))
			return (1);
		if (ft_strexact("cd", cmd->args[0]))
			return (1);
		if (ft_strexact("export", cmd->args[0]))
			return (1);
		if (ft_strexact("unset", cmd->args[0]))
			return (1);
		if (ft_strexact("exit", cmd->args[0]))
			return (1);
		if (ft_strexact("echo", cmd->args[0]))
			return (1);
	}
	return (0);
}

int	check_if_builtin_2pipe(t_cmd *cmd)
{
	if (ft_strexact("pwd", cmd->args[0]))
		return (1);
	if (ft_strexact("echo", cmd->args[0]))
		return (1);
	if (ft_strexact("env", cmd->args[0]))
		return (1);
	return (0);
}

int	is_valid_env_var_name(char **name_value)
{
	int	i;

	i = 0;
	if (ft_isdigit(name_value[0][0]))
	{
		printf("minishell: export \"%s\": not a valid identifier\n",
			name_value[0]);
		ft_matrix_free(name_value);
		return (0);
	}
	else
	{
		while (name_value[0][i] != '\0')
		{
			if (!(ft_isalnum(name_value[0][i++])))
			{
				printf("minishell: export \"%s\": not a valid identifier\n",
					name_value[0]);
				ft_matrix_free(name_value);
				return (0);
			}
		}
	}
	return (1);
}
