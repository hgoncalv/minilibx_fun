/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:25 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/06 21:18:43 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(char **argv)
{
	char	*folder;
	int		to_free;
	char	pwd[256];
	int		ret;

	if (argv[1] && argv[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (getcwd(pwd, sizeof(pwd)) == NULL)
		perror("getcwd() error");
	to_free = set_cd_folder_return_if_free_folder_or_not(argv, &folder);
	ret = chdir(folder);
	if (ret != 0)
		perror("minishell");
	ft_setenv("OLDPWD", pwd, 1);
	if (getcwd(pwd, sizeof(pwd)) == NULL)
		perror("getcwd() error");
	ft_setenv("PWD", pwd, 1);
	if (to_free)
		free(folder);
	return (ret * (-1));
}

int	ft_echo(char **argv)
{
	int	i;
	int	flag;

	flag = 0;
	i = 1;
	while (argv[i] != NULL && ft_strexact(argv[i], "-n"))
		i++;
	if (ft_strexact(argv[i - 1], "-n"))
		flag = 1;
	while (argv[i] != NULL)
	{
		ft_putstr_fd(argv[i], 1);
		i++;
		if (argv[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (!flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

int	ft_pwd(char **argv)
{
	char	cwd[256];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd() error");
	else
		printf("%s\n", cwd);
	return (0);
}

int	ft_export_loop(char **argv)
{
	char	**name_value;

	if (argv[0] && argv[0][0] != '#')
	{
		name_value = ft_strsplit(*argv, '=');
		if (name_value[0] && !is_valid_env_var_name(name_value))
			return (1);
		else
		{
			if (name_value[1])
				ft_setenv(name_value[0], name_value[1], 1);
			else
			{
				if (name_value[0] && ft_strchr(*argv, '=') == NULL)
					ft_setenv(name_value[0], NULL, 1);
				else
					ft_setenv(name_value[0], "=", 1);
			}
			ft_matrix_free(name_value);
			return (0);
		}
	}
	return (1);
}

int	ft_export(char **argv, int index)
{
	int	ret;

	ret = 0;
	if (*argv && ft_strlen(*argv) == 0)
		return (1);
	argv = argv + index;
	if (!*argv || argv[0][0] == '#')
		ft_export_no_args();
	else
	{
		while (*argv != NULL)
		{
			if (ft_export_loop(argv))
				ret = 1;
			argv++;
		}
	}
	return (ret);
}
