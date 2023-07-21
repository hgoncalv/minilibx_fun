/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:05:08 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 11:06:38 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	ft_exit_key(t_game *game)
{
	(void)game;
	exit_msg("Cub3D exited");
	exit(0);
}

void	error_msg(char *str)
{
	ft_putstr(str);
	ft_putchar('\n');
	exit(-1);
}

void	exit_msg(char *str)
{
	ft_putstr(str);
	ft_putchar('\n');
	exit(0);
}

void	free_tab2(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

double	degree_to_radian(double degree)
{
	return (degree * (PI / 180.0));
}
