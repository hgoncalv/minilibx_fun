/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:00:48 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 21:26:47 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isint(char *nbr)
{
	int		i;
	char	*test_nbr;
	int		is_int;
	int		has_sign;

	has_sign = 0;
	if (!ft_isnbr(nbr))
		return (0);
	i = 0;
	while (ft_isspace(nbr[i]))
		i++;
	if (nbr[i] == '+' || nbr[i] == '-')
	{
		has_sign = 1;
		i++;
	}
	test_nbr = (char *)ft_itoa(ft_atoi(nbr));
	if (!test_nbr)
		return (0);
	if (ft_strncmp(nbr + i - has_sign, test_nbr, ft_strlen(nbr + i - has_sign)))
		is_int = 0;
	else
		is_int = 1;
	free(test_nbr);
	return (is_int);
}
