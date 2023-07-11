/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:00:24 by hgoncalv          #+#    #+#             */
/*   Updated: 2022/12/22 11:03:07 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	size_t				i;
	const unsigned char	*cs1;
	const unsigned char	*cs2;

	cs1 = (const unsigned char *)str1;
	cs2 = (const unsigned char *)str2;
	i = 0;
	while (i < n)
	{
		if (cs1[i] != cs2[i])
			return ((unsigned char)cs1[i] - (unsigned char)cs2[i]);
		i++;
	}
	return (0);
}
