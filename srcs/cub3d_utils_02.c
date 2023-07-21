/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_utils_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:04:51 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 17:01:27 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	map_component_check(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ft_strchr("01NSEW ", line[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

int	empty_line_check(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	malloc_buffer(t_game *game)
{
	int	i;

	game->buf = ft_calloc(game->info.win_height + 1, sizeof(int **));
	if (!(game->buf))
		free_game(game,1,"ERROR: Buffer malloc failed");
	i = 0;
	while (i < game->info.win_height)
	{
		game->buf[i] = ft_calloc(game->info.win_width + 1, sizeof(int *));
		if (!(game->buf[i]))
			free_game(game,1,"ERROR: Buffer malloc failed");
		i++;
	}
}

void	malloc_z_buffer(t_game *game)
{
	game->z_buffer = (double *)ft_calloc(game->info.win_width, sizeof(double));
	if (!(game->z_buffer))
		free_game(game,1,"ERROR: z_buffer malloc failed");
}
