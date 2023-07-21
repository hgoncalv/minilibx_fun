/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr_cub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:07:25 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 11:08:54 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strstr_cub(char *str, char *cub)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (ft_strcmp(str, cub) == 0 || cub[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		while (str[i + j] == cub[j])
		{
			if (cub[j + 1] == '\0' && str[i + j + 1] == '\0')
				return (1);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}
