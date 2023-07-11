/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stradd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:00:01 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 16:17:33 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_stradd(char **str, char *add)
{
	int	i;

	if (*str == NULL)
		return ;
	i = 0;
	while ((*str)[i])
		i++;
	(*str) = ft_realloc((*str), ft_strlen(*str) + ft_strlen(add) + 1);
	if (!*str)
		return ;
	while (*add)
	{
		(*str)[i++] = *add;
		add++;
	}
	(*str)[i] = 0;
}
