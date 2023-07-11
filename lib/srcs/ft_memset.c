/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:00:08 by hgoncalv          #+#    #+#             */
/*   Updated: 2022/12/22 11:00:14 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *str, char c, size_t l)
{
	size_t	i;
	char	*s;

	s = (char *)str;
	i = 0;
	while (i < l)
		s[i++] = c;
	return (s);
}
