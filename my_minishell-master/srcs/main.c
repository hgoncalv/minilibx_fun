/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:50 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 02:01:49 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_remove_qt(void)
{
	t_cmd	*ptr;
	int		i;

	ptr = _shell()->head;
	while (ptr)
	{
		i = 0;
		while (ptr->args[i])
		{
			parse_clean(&(ptr->args[i]));
			i++;
		}
		ptr = ptr->next;
	}
	return (1);
}

void	run_minishell(char **line, char ***matrix)
{
	_shell()->p2matrix = matrix;
	add_cmds(*matrix);
	init_remove_qt();
	if (_shell()->head && _shell()->head->args[0])
		pipe_commands(_shell()->head);
	minishell_clean(line);
}

void	minishell(void)
{
	char	*line;
	char	**matrix;

	while (!_shell()->exit)
	{
		signal4main();
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			return ;
		}
		add_history(line);
		remove_start_end_spaces(&line);
		line = ft_var_expansion(line);
		matrix = line_to_matrix(&line);
		if (!matrix)
		{
			minishell_clean(&line);
			continue ;
		}
		run_minishell(&line, &matrix);
	}
	minishell_clean(&line);
}

int	main(int ac, char **av, char **ev)
{
	char	*shlvl;

	(void)ac;
	(void)av;
	if (ac != 1)
		return (0);
	init_shell(ev);
	_shell()->exit_code = 0;
	shlvl = ft_itoa(ft_atoi(getenv("SHLVL")) + 1);
	ft_setenv("SHLVL", shlvl, 1);
	free(shlvl);
	minishell();
	close_shell();
	return (_shell()->exit_code);
}
