/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgoncalv <hgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 11:04:29 by hgoncalv          #+#    #+#             */
/*   Updated: 2023/07/21 17:38:36 by hgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	handle_mouse(t_game	*game)
{
	static int	prev_x = 0;
	int			x;
	int			y;
	int			delta_x;

	mlx_mouse_get_pos(game->mlx, game->win, &x, &y);
	delta_x = x - prev_x;
	prev_x = x;
	if (delta_x == 0)
		return (0);
	if (delta_x > 0)
		rotate_left(&game->player);
	else if (delta_x < 0)
		rotate_right(&game->player);
	game->to_render = 1;
	return (0);
}

int	main_loop(t_game *game)
{
	handle_mouse(game);
	if (!game->to_render)
		return (0);
	ray_processing(game);
	render_screen(game);
	game->to_render = 0;
	return (1);
}

void	game_set_go(t_game *game)
{
	game->win = mlx_new_window(game->mlx, game->info.win_width,
			game->info.win_height, TITLE);
	mlx_hook(game->win, X_EVENT_KEY_EXIT, 1L << 17, &ft_exit_key, game);
	mlx_hook(game->win, X_EVENT_KEY_PRESS, 1L << 0, &ft_keypress, game);
	mlx_loop_hook(game->mlx, &main_loop, game);
	mlx_loop(game->mlx);
}

int	main(int ac, char **av)
{
	t_game	game;

	file_validation_check(ac, av);
	all_init(&game);
	find_info(&game, av);
	info_validation_check(&game);
	read_map(&game, game.info.line);
	map_validation_check(&game);
	game_init(&game);
	game_set_go(&game);
	return (EXIT_SUCCESS);
}
