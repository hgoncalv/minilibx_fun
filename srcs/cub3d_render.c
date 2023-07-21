/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:05:13 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 11:06:37 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	render_ceiling_and_floor(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->info.win_height - 1)
	{
		x = 0;
		while (x < game->info.win_width)
		{
			if (y > game->info.win_height / 2)
				game->buf[y][x] = game->info.floor_color;
			else
				game->buf[y][x] = game->info.ceiling_color;
			x++;
		}
		y++;
	}
}

void	render_screen(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->info.win_height)
	{
		x = 0;
		while (x < game->info.win_width)
		{
			game->img.data[game->info.win_width * y + x] = game->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img.img_ptr, 0, 0);
}
