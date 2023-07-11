/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 15:20:12 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/01/05 20:29:12 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_swap2str(char **str1, char **str2)
{
	char	*tmp_str;

	tmp_str = ft_strdup(*str1);
	free(*str1);
	*str1 = ft_strdup(*str2);
	free(*str2);
	*str2 = ft_strdup(tmp_str);
	free(tmp_str);
}
