/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:03 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/05 22:46:51 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_get_exec_path_chk_access(char *path, char **env_path, char ***matrix)
{
	if (access(path, F_OK) != -1)
	{
		free(*env_path);
		ft_matrix_free(*matrix);
		return (1);
	}
	return (0);
}

char	*get_exec_path_all_getenv(t_vars *v, char **env_path)
{
	v->tmp_str = ft_getenv("PATH", 1);
	if (!v->tmp_str)
		return (NULL);
	*env_path = ft_strdup(v->tmp_str);
	free(v->tmp_str);
	v->matrix = ft_strsplit(*env_path, ':');
	return (*env_path);
}

char	*ft_get_exec_path(char **argv)
{
	char	*env_path;
	char	*path;
	t_vars	v;

	ft_inicialize_vars(&v);
	if (argv[0] && (argv[0][0] == '.' || argv[0][0] == '/'))
	{
		if (access(argv[0], F_OK) != -1)
			return (ft_strdup(argv[0]));
		return (NULL);
	}
	if (!get_exec_path_all_getenv(&v, &env_path))
		return (NULL);
	while (v.matrix[v.i++] != NULL)
	{
		path = ft_concat3(v.matrix[v.i - 1], "/", argv[0]);
		if (ft_get_exec_path_chk_access(path, &env_path, &(v.matrix)))
			return (path);
		free(path);
	}
	free(env_path);
	free_arrays(&v.matrix);
	return (NULL);
}

char	*ft_getenv(char *name, int trimmed)
{
	int		i;
	char	**split;
	char	*ret;

	i = -1;
	if (ft_strexact(name, "?"))
		return (ft_itoa(_shell()->exit_code));
	while (_shell()->envp[++i] != NULL && name && ft_strncmp(_shell()->envp[i],
			name, ft_strlen(name)))
		;
	if (_shell()->envp[i] && _shell()->envp[i][ft_strlen(name)] != '=')
		return (NULL);
	if (_shell()->envp[i] != NULL)
	{
		split = ft_strsplit(_shell()->envp[i], '=');
		if (split[1] == NULL)
			ret = NULL;
		else if (!trimmed)
			ret = ft_concat_string_between_chars(2, split[1], 2);
		else
			ret = ft_strdup(split[1]);
		ft_matrix_free(split);
		return (ret);
	}
	return (NULL);
}
