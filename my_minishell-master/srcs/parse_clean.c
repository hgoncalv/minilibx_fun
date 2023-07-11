/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:19:57 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/10 01:54:52 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*parse_clean(char **p2str)
{
	t_vars	v;
	int		i;
	char	c;

	ft_inicialize_vars(&v);
	v.str = *p2str;
	(v.str2) = ft_realloc((v.str2), sizeof(char) * ft_strlen((v.str)) + 1);
	i = 0;
	while (*(v.str))
	{
		if (*(v.str) == 2 || *(v.str) == '\'' || *(v.str) == '"')
		{
			c = *(v.str)++;
			while (*(v.str) && *(v.str) != c)
				(v.str2)[i++] = *((v.str)++);
		}
		else
			(v.str2)[i++] = *(v.str);
		if (*(v.str))
			(v.str)++;
	}
	(v.str2)[i] = 0;
	free(*p2str);
	*p2str = (v.str2);
	return ((v.str2));
}

char	check_if_str_need_quotes(char *str)
{
	int		i;
	int		f;
	char	c;
	char	k;

	f = 0;
	k = 0;
	i = -1;
	c = 0;
	while (str[++i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (f++ == 0)
				c = str[i];
		}
		if (str[i] == c)
			k++;
	}
	if (k % 2 != 0)
	{
		printf("SYNTAX ERROR: unclosed quotes ...\n");
		return (c);
	}
	return (0);
}

void	check_rowquotes_free_if_needed(char **row)
{
	if (check_if_str_need_quotes(*row))
	{
		free(*row);
		*row = NULL;
	}
}
