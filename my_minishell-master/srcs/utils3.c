/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:12 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/06 17:12:10 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_char(char **str, char c)
{
	int		str_len;
	char	*temp;
	int		i;
	int		j;

	str_len = strlen(*str);
	temp = NULL;
	temp = ft_realloc(temp, str_len + 1);
	i = 0;
	j = 0;
	while ((*str)[i] != '\0')
	{
		if ((*str)[i] != c)
		{
			temp[j] = (*str)[i];
			j++;
		}
		i++;
	}
	temp[j] = '\0';
	free(*str);
	*str = temp;
}
