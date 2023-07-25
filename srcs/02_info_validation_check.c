/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_info_validation_check.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:06:18 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/25 12:15:36 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	file_validation_check(int ac, char **av)
{
	if (ac != 2)
		error_msg("Argument Not Enough or Too Many");
	if (ac == 2 && (ft_strstr_cub(av[1], ".cub")) != 1)
		error_msg("Wrong .cub Extension");
}

void	info_validation_check(t_game *game)
{
	if (game->info.mark == -1)
		free_game(game, 1,
			"Info element(s) Missing or Info not well aligned");
	if (!game->info.win_width || !game->info.win_height)
		free_game(game, 1, "Resolution Info missing");
	if (game->info.floor_color == -1 || game->info.ceiling_color == -1)
		free_game(game, 1, "Color Info missing");
	if (!game->info.tex_path[TEX_NORTH] || !game->info.tex_path[TEX_SOUTH]
		|| !game->info.tex_path[TEX_WEST] || !game->info.tex_path[TEX_EAST])
		free_game(game, 1, "Texture Info missing");
}

	// if (check == 1)
	// 	printf("INFO: Window resized as : %d %d\n", game->info.win_width,
	// 		game->info.win_height);
void	linux_resolution_max_min_check(t_game *game)
{
	int	r_width;
	int	r_height;
	int	check;

	r_width = 0;
	r_height = 0;
	check = 0;
	mlx_get_screen_size(game->mlx, &r_width, &r_height);
	if (game->info.win_width > r_width || game->info.win_width < 0)
	{
		game->info.win_width = r_width;
		check = 1;
	}
	if (game->info.win_height > r_height || game->info.win_height < 0)
	{
		game->info.win_height = r_height;
		check = 1;
	}
	(void)check;
}
